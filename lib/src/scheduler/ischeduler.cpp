#include <scheduler/ischeduler.h>
#include <compose/manager.h>
#include <chrono>

using namespace std::chrono;

// std::map<compose::engine, std::map<std::string, std::shared_ptr<iengine>>> data;
void ischeduler::init(const std::vector<std::string> models, int queue_size) { 

    auto engines = compose::manager::instance()->engine_list();
    spdlog::info("ischeduler::init");

    auto s = high_resolution_clock::now();
    for (const auto& e : engines) { 
        this->data[e] = std::make_shared<buf_pu_queue>(queue_size);
        volatile bool f = false;
        
        this->data[e]->run_loop(models, e, [&f]() { 
            printf("콜백\n");
            f = true;
        });
        printf("챠챠 [ %s ] \n", data[e]->get_name().c_str());
    
        while(!f);
    }
    auto e = high_resolution_clock::now();

    spdlog::info("fin : {} ns", (e - s).count());
}
