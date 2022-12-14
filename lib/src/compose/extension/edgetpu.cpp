#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

/* for Tensorflow Lite */
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/kernels/register.h>

#include <edgetpu.h>
#include <edgetpu_c.h>

// using namespace std::chrono;

namespace tpu {
constexpr size_t kBmpFileHeaderSize = 14;
constexpr size_t kBmpInfoHeaderSize = 40;
constexpr size_t kBmpHeaderSize = kBmpFileHeaderSize + kBmpInfoHeaderSize;

int32_t ToInt32(const char p[4]) {
  return (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
}

std::vector<uint8_t> ReadBmpImage(const char* filename,
                                  int* out_width = nullptr,
                                  int* out_height = nullptr,
                                  int* out_channels = nullptr) {
  assert(filename);

  std::ifstream file(filename, std::ios::binary);
  if (!file) return {};  // Open failed.

  char header[kBmpHeaderSize];
  if (!file.read(header, sizeof(header))) return {};  // Read failed.

  const char* file_header = header;
  const char* info_header = header + kBmpFileHeaderSize;

  if (file_header[0] != 'B' || file_header[1] != 'M')
    return {};  // Invalid file type.

  const int channels = info_header[14] / 8;
  if (channels != 1 && channels != 3) return {};  // Unsupported bits per pixel.

  if (ToInt32(&info_header[16]) != 0) return {};  // Unsupported compression.

  const uint32_t offset = ToInt32(&file_header[10]);
  if (offset > kBmpHeaderSize &&
      !file.seekg(offset - kBmpHeaderSize, std::ios::cur))
    return {};  // Seek failed.

  int width = ToInt32(&info_header[4]);
  if (width < 0) return {};  // Invalid width.

  int height = ToInt32(&info_header[8]);
  const bool top_down = height < 0;
  if (top_down) height = -height;

  const int line_bytes = width * channels;
  const int line_padding_bytes =
      4 * ((8 * channels * width + 31) / 32) - line_bytes;
  std::vector<uint8_t> image(line_bytes * height);
  for (int i = 0; i < height; ++i) {
    uint8_t* line = &image[(top_down ? i : (height - 1 - i)) * line_bytes];
    if (!file.read(reinterpret_cast<char*>(line), line_bytes))
      return {};  // Read failed.
    if (!file.seekg(line_padding_bytes, std::ios::cur))
      return {};  // Seek failed.
    if (channels == 3) {
      for (int j = 0; j < width; ++j) std::swap(line[3 * j], line[3 * j + 2]);
    }
  }

  if (out_width) *out_width = width;
  if (out_height) *out_height = height;
  if (out_channels) *out_channels = channels;
  return image;
}

std::vector<std::string> ReadLabels(const std::string& filename) {
  std::ifstream file(filename);
  if (!file) return {};  // Open failed.

  std::vector<std::string> lines;
  for (std::string line; std::getline(file, line);) lines.emplace_back(line);
  return lines;
}

std::string GetLabel(const std::vector<std::string>& labels, int label) {
  if (label >= 0 && label < labels.size()) return labels[label];
  return std::to_string(label);
}

std::vector<float> Dequantize(const TfLiteTensor& tensor) {
  const auto* data = reinterpret_cast<const uint8_t*>(tensor.data.data);
  std::vector<float> result(tensor.bytes);
  for (int i = 0; i < tensor.bytes; ++i)
    result[i] = tensor.params.scale * (data[i] - tensor.params.zero_point);
  return result;
}

std::vector<std::pair<int, float>> Sort(const std::vector<float>& scores,
                                        float threshold) {
  std::vector<const float*> ptrs(scores.size());
  std::iota(ptrs.begin(), ptrs.end(), scores.data());
  auto end = std::partition(ptrs.begin(), ptrs.end(),
                            [=](const float* v) { return *v >= threshold; });
  std::sort(ptrs.begin(), end,
            [](const float* a, const float* b) { return *a > *b; });

  std::vector<std::pair<int, float>> result;
  result.reserve(end - ptrs.begin());
  for (auto it = ptrs.begin(); it != end; ++it)
    result.emplace_back(*it - scores.data(), **it);
  return result;
}
}  // namespace
