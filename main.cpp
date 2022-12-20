#include <iostream>
#include <compose/manager.h>
#include <chrono>
#include <thread>
#include <unistd.h>

using namespace std::chrono;

int main(int argc, char **argv)
{
    auto e = compose::manager::instance()->inference_engine();
    compose::model_info d;
    d.batch = 1;
    d.channel = 3;
    d.height = 224;
    d.width = 224;
    d.layout = compose::data_layout::nhwc;

    if (e.size() == 0) { 
        printf("not found inference engine");
        return 0;
    }

    std::vector<std::thread> chacha;
    for (int i = 0; i < e.size() - 1; i++) {
        int p = i;
        chacha.push_back(std::thread([d, p, &e]() { 
            std::cout << e[p]->get_name() << " : start\n";
            e[p]->init("vgg16", d);
            std::cout << e[p]->get_name() << " : end\n";
            
            for(;;){
                auto start = high_resolution_clock::now();
                e[p]->inference("./dog_224x224.jpg");
                std::cout << e[p]->get_name() << " : " << (high_resolution_clock::now() - start).count() / 1000 << "us\n";
            }
        }));
    }

    std::cout << e[e.size() - 1]->get_name() << " : start\n";
    e[e.size() - 1]->init("vgg16", d);
    std::cout << e[e.size() - 1]->get_name() << " : end\n";

    for(;;){
        auto start = high_resolution_clock::now();
        e[e.size() - 1]->inference("./dog_224x224.jpg");
        std::cout << e[e.size() - 1]->get_name() << " : " << (high_resolution_clock::now() - start).count() / 1000 << "us\n";
    }
    
    return 0;
}
