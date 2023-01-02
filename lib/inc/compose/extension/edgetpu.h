#pragma once

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

int32_t ToInt32(const char p[4]);

std::vector<uint8_t> ReadBmpImage(const char* filename,
                                  int* out_width = nullptr,
                                  int* out_height = nullptr,
                                  int* out_channels = nullptr);

std::vector<std::string> ReadLabels(const std::string& filename);

std::string GetLabel(const std::vector<std::string>& labels, int label);

std::vector<float> Dequantize(const TfLiteTensor& tensor);

std::vector<std::pair<int, float>> Sort(const std::vector<float>& scores,
                                        float threshold);
}  // namespace
