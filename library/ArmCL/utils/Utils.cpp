/*
 * Copyright (c) 2017-2019 Arm Limited.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "Utils.h"

#ifdef ARM_COMPUTE_CL
#include "arm_compute/core/CL/CLKernelLibrary.h"
#include "arm_compute/runtime/CL/CLScheduler.h"
#endif /* ARM_COMPUTE_CL */
#include "arm_compute/runtime/NEON/NEScheduler.h"
#include <cctype>
#include <cerrno>
#include <iomanip>
#include <string>
#include <vector>
#include <chrono>
#include <memory>
#include <unistd.h>
#include <algorithm>
#include <tuple>
#include <thread>

#include <fstream>

#include "../tuner/strategy_factory.h"

using namespace std::chrono;
using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#if (defined(__GNUC__) && (__GNUC__ >= 7))
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif // (defined(__GNUC__) && (__GNUC__ >= 7))
#if defined(__clang__)
#pragma GCC diagnostic ignored "-Wparentheses-equality"
#endif // defined(__clang__)
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#pragma GCC diagnostic pop

namespace arm_compute
{
namespace utils
{
namespace
{
/* Advance the iterator to the first character which is not a comment
 *
 * @param[in,out] fs Stream to drop comments from
 */
void discard_comments(std::ifstream &fs)
{
    while(fs.peek() == '#')
    {
        fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

/* Advance the string iterator to the next character which is neither a space or a comment
 *
 * @param[in,out] fs Stream to drop comments from
 */
void discard_comments_and_spaces(std::ifstream &fs)
{
    while(true)
    {
        discard_comments(fs);

        if(isspace(fs.peek()) == 0)
        {
            break;
        }

        fs.ignore(1);
    }
}
} // namespace

#ifndef BENCHMARK_EXAMPLES
int run_example(int argc, char **argv, std::unique_ptr<Example> example)
{
    std::cout << "\n"
              << argv[0] << "\n\n";

    int mode = 0;
    std::string kernelOps = "";
    if (argc >= 4){ 
        mode = std::stoi(argv[3]);
    }
    if (argc >= 5){ 
        kernelOps = argv[4];
    }

    arm_compute::Scheduler::get().generate_core_thread();
    arm_compute::Scheduler::get().set_gemm_kernelOps(kernelOps);
    arm_compute::Scheduler::get().set_conv_method(mode);

    std::cout << "ADASDAS " << arm_compute::Scheduler::get().get_gemm_kerenlOps() << "\n";
    
    bool status = example->do_setup(argc, argv);

    std::vector<std::vector<double>> data;

    int count = 0;
    int repeat_count = 0;

    std::vector<unsigned int> measure_time;

    std::shared_ptr<tuner> tun = nullptr;
    tuner_data current;
    int layer_idx = 1;
    
   

    std::vector<std::string> filter_kernel = { "CpuIm2ColKernel", "CpuCol2ImKernel", 
                                                "CpuGemmAssemblyWrapperKernel", "CpuDirectConvolutionLayerKernel",
                                                "CpuWinogradConv2dTransformOutputKernel", "CpuWinogradConv2dTransformInputKernel", 
                                                "CpuDirectConv2dOutputStageKernel" };
    // strategy_factory::instance();
    
    int call_count = 0;

    arm_compute::Scheduler::get().set_tuner_info([&tun, &count, &layer_idx, mode, &filter_kernel, &call_count,
                                                    &current, &repeat_count, &measure_time](const char* kernel_name, int max_window) {
        using namespace std::this_thread;
        using std::chrono::system_clock;
        if (REMOVE_CACHE) { 
            system("sync; echo 3 > /proc/sys/vm/drop_caches");
        }
        sleep_until(system_clock::now() + milliseconds(SEARCH_EACH_DELAY_UNIT_MS));
        call_count += 1;
        if (call_count > 8 * 2000) { 
            sleep_until(system_clock::now() + seconds(60));
            call_count = 0;
        }
        // default run
        if (count == 0) { 
            current = tuner_data{};
            current.windows = std::vector<int>();
            current.threads = std::vector<int> { 4, 5 };
            if (repeat_count >= SEARCH_RUN_REPEAT) { 
                count += 1;
                repeat_count = 0;
                measure_time.clear();
            }else { 
                repeat_count += 1;
            }
            return true;
        }

        // Convolution Optimization Mode
        if (mode != 0 && count == 1) { 
            bool is_skip = true;
            for (const auto& check_item : filter_kernel) { 
                std::string filter = kernel_name;
                int idx = filter.find('/');
                if (idx != std::string::npos) { 
                    filter = filter.substr(0, idx);
                }
                if (filter == check_item) { 
                    is_skip = false;
                }
            }
            if (is_skip) { 
                count = 0;
                repeat_count = 0;
                layer_idx += 1;
                measure_time.clear();
                return false;
            }
        }else if (mode == 0 && count == 1) { 
            bool is_skip = false;
            for (const auto& check_item : filter_kernel) { 
                std::string filter = kernel_name;
                int idx = filter.find('/');
                if (idx != std::string::npos) { 
                    filter = filter.substr(0, idx);
                }
                if (filter == check_item) { 
                    is_skip = true;
                }
            }
            if (is_skip) { 
                count = 0;
                repeat_count = 0;
                layer_idx += 1;
                measure_time.clear();
                return false;
            }
        }

        if (tun == nullptr) { 
            tun = strategy_factory::instance()->grid(max_window, SEARCH_MAX_RUN_COUNT);
            
            auto p = tun->get_next();
            count += 1;
            current = p[0];
            return true;
        }
        if (count > SEARCH_MAX_RUN_COUNT || !tun->is_next()) { 
            tun = nullptr;
            layer_idx += 1;
            count = 0;
            return false;
        }else { 
            if (repeat_count >= SEARCH_RUN_REPEAT) { 
                auto p = tun->get_next();
                measure_time.clear();
                current = p[0];
                count += 1;
                repeat_count = 0;
            }else { 
                repeat_count += 1;
            }
            return true;
        }
    },
    [&current, &tun, &layer_idx, &count](){
        return current.threads;
    },
    [&current](int idx, int max_idx, int max_window, int start, int end, int step) -> std::tuple<int, int, int> {
        if (current.windows.size() == 0) { 
            return std::make_tuple(-1,-1,-1);
        }else { 
            int sum = 0;
            int _start = 0;
            int _end = 0;
            for (int i = 0; i < idx; i++) { 
                _start += current.windows[i];
            }
            _end = _start;
            _end += current.windows[idx];

            return std::make_tuple(_start * step, _end * step, step);
        }
    }, 
    [&current, &layer_idx, &measure_time, &repeat_count, mode, &filter_kernel](std::string kernelName, unsigned int measure_speed) -> void {
        measure_time.push_back(measure_speed);
        
        bool is_conv = false;
        for (const auto& check_item : filter_kernel) { 
            std::string filter = kernelName;
            int idx = filter.find('/');
            if (idx != std::string::npos) { 
                filter = filter.substr(0, idx);
            }
            if (filter == check_item) { 
                is_conv = true;
            }
        }

        auto idx = kernelName.find("/");
        if (idx != std::string::npos) { 
            kernelName.replace(idx, 1, "_");
        }

        
        if (repeat_count == SEARCH_RUN_REPEAT - 1) {
            std::string fileName;
            if (is_conv) { 
                fileName = std::to_string(layer_idx) + "_" + kernelName + "_" + std::to_string(mode) + ".csv";
            }else { 
                fileName = std::to_string(layer_idx) + "_" + kernelName + ".csv";
            }
            std::ofstream output(fileName, fstream::out | fstream::app);
            // print start threads
            
            std::cout << kernelName << "," << (int)current.thread_mode << ",";
            output << kernelName << "," << (int)current.thread_mode << ",";

            for (const auto& item : measure_time) { 
                std::cout << item << ",";   
                output << item << ",";
            }

            for (const auto& th : current.threads) { 
                output << th << ",";
                std::cout << th << ",";
            }
            output << "slice,";
            std::cout << "slice,";
            for (const auto& th : current.windows) { 
                output << th << ",";
                std::cout << th << ",";
            }
            output << "\n";
            std::cout << "\n";
            output.close();
        }
        return;
    });
    
    // arm_compute::Scheduler::get().reset_window_result();

    example->do_run(0);
    
    // auto window = arm_compute::Scheduler::get().get_window_result();
    // for (auto& item : window){  
    //     std::cout << item << " ";
    // }
    // std::cout << "\n";

    // for (int d = 0; d < 4; d++) { 
    //     double avg = 0;
    //     switch (d) { 
    //     case 0:
    //         std::cout << "Little : " << std::endl;
    //         break;
    //     case 1:
    //         std::cout << "Big : " << std::endl;
    //         break;
    //     case 2:
    //         std::cout << "All : " << std::endl;
    //         break;
    //     case 3:
    //         std::cout << "Best : " << std::endl;
    //         for (int i = 0; i < data[0].size(); i += 8) { 
    //             std::array<double, 3> arr = { 0, 0, 0 };

    //             for (int t = 0; t < data[0].size() / 8; t++) { 
    //                 arr[0] += data[0][i * 8 + t] / 8;
    //                 arr[1] += data[1][i * 8 + t] / 8;
    //                 arr[2] += data[2][i * 8 + t] / 8;
    //             }
    //             std::sort(arr.begin(), arr.end());
    //             avg += arr[0];
    //         }
    //         std::cout << "Avg : [ " << avg / 1000.0 / 1000.0 << " ] " << std::endl;
    //         continue;
    //     }

    //     for (int i = 0; i < data[d].size(); i++) { 
    //         avg += data[d][i] / 1000.0 / 1000.0;
    //     }
    //     std::cout << "Avg : [ " << avg / 8 << " ] " << std::endl;
    // }

    
    example->do_teardown();

    std::cout << "\nTest passed\n";
    return 0;
}

#endif /* BENCHMARK_EXAMPLES */

void draw_detection_rectangle(ITensor *tensor, const DetectionWindow &rect, uint8_t r, uint8_t g, uint8_t b)
{
    ARM_COMPUTE_ERROR_ON_FORMAT_NOT_IN(tensor, Format::RGB888);

    uint8_t *top    = tensor->info()->offset_element_in_bytes(Coordinates(rect.x, rect.y)) + tensor->buffer();
    uint8_t *bottom = tensor->info()->offset_element_in_bytes(Coordinates(rect.x, rect.y + rect.height)) + tensor->buffer();
    uint8_t *left   = top;
    uint8_t *right  = tensor->info()->offset_element_in_bytes(Coordinates(rect.x + rect.width, rect.y)) + tensor->buffer();
    size_t   stride = tensor->info()->strides_in_bytes()[Window::DimY];

    for(size_t x = 0; x < rect.width; ++x)
    {
        top[0]    = r;
        top[1]    = g;
        top[2]    = b;
        bottom[0] = r;
        bottom[1] = g;
        bottom[2] = b;

        top += 3;
        bottom += 3;
    }

    for(size_t y = 0; y < rect.height; ++y)
    {
        left[0]  = r;
        left[1]  = g;
        left[2]  = b;
        right[0] = r;
        right[1] = g;
        right[2] = b;

        left += stride;
        right += stride;
    }
}

ImageType get_image_type_from_file(const std::string &filename)
{
    ImageType type = ImageType::UNKNOWN;

    try
    {
        // Open file
        std::ifstream fs;
        fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fs.open(filename, std::ios::in | std::ios::binary);

        // Identify type from magic number
        std::array<unsigned char, 2> magic_number{ { 0 } };
        fs >> magic_number[0] >> magic_number[1];

        // PPM check
        if(static_cast<char>(magic_number[0]) == 'P' && static_cast<char>(magic_number[1]) == '6')
        {
            type = ImageType::PPM;
        }
        else if(magic_number[0] == 0xFF && magic_number[1] == 0xD8)
        {
            type = ImageType::JPEG;
        }

        fs.close();
    }
    catch(std::runtime_error &e)
    {
        ARM_COMPUTE_ERROR_VAR("Accessing %s: %s", filename.c_str(), e.what());
    }

    return type;
}

std::tuple<unsigned int, unsigned int, int> parse_ppm_header(std::ifstream &fs)
{
    // Check the PPM magic number is valid
    std::array<char, 2> magic_number{ { 0 } };
    fs >> magic_number[0] >> magic_number[1];
    ARM_COMPUTE_ERROR_ON_MSG(magic_number[0] != 'P' || magic_number[1] != '6', "Invalid file type");
    ARM_COMPUTE_UNUSED(magic_number);

    discard_comments_and_spaces(fs);

    unsigned int width = 0;
    fs >> width;

    discard_comments_and_spaces(fs);

    unsigned int height = 0;
    fs >> height;

    discard_comments_and_spaces(fs);

    int max_val = 0;
    fs >> max_val;

    discard_comments(fs);

    ARM_COMPUTE_ERROR_ON_MSG(isspace(fs.peek()) == 0, "Invalid PPM header");
    fs.ignore(1);

    return std::make_tuple(width, height, max_val);
}

std::tuple<std::vector<unsigned long>, bool, std::string> parse_npy_header(std::ifstream &fs) //NOLINT
{
    std::vector<unsigned long> shape; // NOLINT

    // Read header
    std::string header = npy::read_header(fs);

    // Parse header
    bool        fortran_order = false;
    std::string typestr;
    npy::parse_header(header, typestr, fortran_order, shape);

    std::reverse(shape.begin(), shape.end());

    return std::make_tuple(shape, fortran_order, typestr);
}

/** This function returns the amount of memory free reading from /proc/meminfo
 *
 * @return The free memory in kB
 */
uint64_t get_mem_free_from_meminfo()
{
    std::string   line_attribute;
    std::ifstream file_meminfo("/proc/meminfo");

    if(file_meminfo.is_open())
    {
        while(!(file_meminfo >> line_attribute).fail())
        {
            //Test if is the line containing MemFree
            if(line_attribute == "MemFree:")
            {
                uint64_t mem_available;
                if(!(file_meminfo >> mem_available).fail())
                {
                    return mem_available;
                }
                else
                {
                    return 0;
                }
            }
            // if it's not MemFree ignore rest of the line
            file_meminfo.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    // Nothing found or an error during opening the file
    return 0;
}
} // namespace utils
} // namespace arm_compute
