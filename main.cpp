#include <iostream>
#include <compose/manager.h>
#include <chrono>

#include <thread>

using namespace std::chrono;


// #include <spdlog/spdlog.h>
// #include <SimpleRKNN/simple_rknn.h>
// #include <ArmCL/arm_compute/runtime/Scheduler.h>


int main(int argc, char **argv)
{
    // auto vino = run_vino();
    // auto rkn = run_rknn();
    // run_neon();
    auto e = compose::manager::instance()->inference_engine();
    
    std::vector<std::thread> chacha;
    for (int i = 0; i < e.size(); i++) {
        int p = i;
        chacha.push_back(std::thread([p, &e]() { 
            std::cout << e[p]->get_name() << " : start\n";
            e[p]->init("resnet50");
            std::cout << e[p]->get_name() << " : end\n";

            for(;;){
                auto start = high_resolution_clock::now();
                e[p]->inference("./dog_224x224.jpg");
                std::cout << e[p]->get_name() << " : " << (high_resolution_clock::now() - start).count() / 1000 << "us\n";
            }
        }));
    }

    while(true);
    return 0;
}
