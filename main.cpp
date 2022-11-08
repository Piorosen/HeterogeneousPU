#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sys/time.h>
#include <chrono>
#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <spdlog/spdlog.h>
#include <SimpleRKNN/simple_rknn.h>
#include <ArmCL/arm_compute/runtime/Scheduler.h>
#include <openvino/openvino.hpp>


using namespace std;
using namespace std::chrono;

int main(int argc, char **argv)
{
    ov::get
    ov::get_openvino_version();
    ov::Core core;
    vector<string> availableDevices = core.get_available_devices();
    cout << "~~~!!";
    for (auto&& device : availableDevices) {
        cout << device << "\n";
    }

    spdlog::info("run!!");
    cout << arm_compute::Scheduler::get().cpu_info().get_cpu_num() << "\n";

    
    return 0;
}
