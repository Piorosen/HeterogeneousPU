#include <scheduler/scheduler.h>

// #include <iostream>
// #include <compose/manager.h>
// #include <chrono>
// #include <thread>
// #include <unistd.h>

// using namespace std::chrono;
// using namespace std;

int main(int argc, char **argv)
{
    auto eft = scheduler::instance()->eft_scheduler();
    
    

    // auto e = compose::manager::instance()->inference_engine();
    // std::cout << e.size() << "\n";
    // for (const auto& item : e) { 
    //   cout << item->get_name() << "\n";
    // }

    // compose::model_info d;
    // d.batch = 1;
    // d.channel = 3;
    // d.height = 224;
    // d.width = 224;
    // d.layout = compose::data_layout::nhwc;
    // // printf("[ %s] ", argv[1]);
    // e[0]->init("ssd_mobilenet_v1int8.tflite", d);
    // auto start = high_resolution_clock::now();

    // for (int i = 0; i < 10; i++) { 
    //   e[0]->inference("00374.jpg");
    // }

    // auto end = high_resolution_clock::now();
    // std::cout << "\n" << (end - start).count() << "ns\n";
    // e[0]->deinit();

    // return 0;

    // if (e.size() == 0) { 
    //     printf("not found inference engine");
    //     return 0;
    // }

    // std::vector<std::thread> chacha;
    // for (int i = 0; i < e.size() - 1; i++) {
    //     int p = i;
    //     chacha.push_back(std::thread([d, p, &e]() { 
    //         std::cout << e[p]->get_name() << " : start\n";
    //         e[p]->init("vgg16", d);
    //         std::cout << e[p]->get_name() << " : end\n";
            
    //         for(;;){
    //             auto start = high_resolution_clock::now();
    //             e[p]->inference("./dog_224x224.jpg");
    //             std::cout << e[p]->get_name() << " : " << (high_resolution_clock::now() - start).count() / 1000 << "us\n";
    //         }
    //     }));
    // }

    // std::cout << e[e.size() - 1]->get_name() << " : start\n";
    // e[e.size() - 1]->init("vgg16", d);
    // std::cout << e[e.size() - 1]->get_name() << " : end\n";

    // for(;;){
    //     auto start = high_resolution_clock::now();
    //     e[e.size() - 1]->inference("./dog_224x224.jpg");
    //     std::cout << e[e.size() - 1]->get_name() << " : " << (high_resolution_clock::now() - start).count() / 1000 << "us\n";
    // }
    
    return 0;
}