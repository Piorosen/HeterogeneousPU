#include <compose/OpenVINO.h>
#include <compose/OpenCL.h>
#include <compose/NEON.h>
#include <compose/RKNN.h>



// #include <spdlog/spdlog.h>
// #include <SimpleRKNN/simple_rknn.h>
// #include <ArmCL/arm_compute/runtime/Scheduler.h>


int main(int argc, char **argv)
{
    // auto vino = run_vino();
    // auto rkn = run_rknn();
    run_neon();

    return 0;
}
