#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sys/time.h>
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <spdlog/spdlog.h>
#include <SimpleRKNN/simple_rknn.h>
#include <ArmCL/arm_compute/runtime/Scheduler.h>

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv)
{
    spdlog::info("run!!");
    cout << arm_compute::Scheduler::get().cpu_info().get_cpu_num() << "\n";
}
