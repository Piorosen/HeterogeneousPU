#include <iostream>
#include <compose/manager.h>
#include <chrono>
#include <thread>
#include <unistd.h>

using namespace std::chrono;

int main(int argc, char **argv)
{
    auto e = compose::manager::instance()->inference_engine();
    
    std::vector<std::thread> chacha;
    volatile bool toggle = false;
    for (int i = 0; i < e.size() - 1; i++) {
        int p = i;
        chacha.push_back(std::thread([&toggle, p, &e]() { 
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

    std::cout << e[e.size() - 1]->get_name() << " : start\n";
    e[e.size() - 1]->init("resnet50");
    std::cout << e[e.size() - 1]->get_name() << " : end\n";

    for(;;){
        auto start = high_resolution_clock::now();
        e[e.size() - 1]->inference("./dog_224x224.jpg");
        std::cout << e[e.size() - 1]->get_name() << " : " << (high_resolution_clock::now() - start).count() / 1000 << "us\n";
    }
    
    return 0;
}
