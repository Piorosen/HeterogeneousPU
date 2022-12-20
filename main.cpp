#include <iostream>
#include <compose/manager.h>
#include <chrono>
#include <thread>

#include <argument/parser.h>

using namespace std::chrono;

int main(int argc, char **argv)
{
    auto opt = parser(argc, argv);
    if (!opt.first) { 
        return 0;
    }
    
    auto e = compose::manager::instance()->inference_engine();
    compose::model_info d;
    d.batch = opt.second.batch;
    d.channel = opt.second.channel;
    d.height = opt.second.height;
    d.width = opt.second.width;

    if (opt.second.data_layout == "nhwc") { 
        d.layout = compose::data_layout::nhwc;
        std::cout << "set : datalayout nhwc\n";
    }else { 
        d.layout = compose::data_layout::nchw;
        std::cout << "set : datalayout nchw\n";
    }

    if (e.size() == 0) { 
        printf("not found inference engine");
        return 0;
    }

    std::vector<std::thread> chacha;
    for (int i = 0; i < e.size() - 1; i++) {
        int p = i;
        chacha.push_back(std::thread([opt, d, p, &e]() { 
            std::cout << e[p]->get_name() << " : start\n";
            e[p]->init(opt.second.model_path, d);
            std::cout << e[p]->get_name() << " : end\n";
            
            for(;;){
                auto start = high_resolution_clock::now();
                e[p]->inference(opt.second.image_path);
                std::cout << e[p]->get_name() << " : " << (high_resolution_clock::now() - start).count() / 1000 << "us\n";
            }
        }));
    }

    std::cout << e[e.size() - 1]->get_name() << " : start\n";
    e[e.size() - 1]->init(opt.second.model_path, d);
    std::cout << e[e.size() - 1]->get_name() << " : end\n";

    for(;;){
        auto start = high_resolution_clock::now();
        e[e.size() - 1]->inference(opt.second.image_path);
        std::cout << e[e.size() - 1]->get_name() << " : " << (high_resolution_clock::now() - start).count() / 1000 << "us\n";
    }
    
    return 0;
}
