#include <scheduler/ischeduler.h>
#include <compose/manager.h>
#include <chrono>

using namespace std::chrono;

// std::map<compose::engine, std::map<std::string, std::shared_ptr<iengine>>> data;
void ischeduler::init(const std::vector<std::string> models) { 

    auto engines = compose::manager::instance()->engine_list();
    spdlog::info("ischeduler::init");

    auto s = high_resolution_clock::now();
    for (const auto& m : models) { 
        for (const auto& e : engines) { 
            auto s1 = high_resolution_clock::now();

            data[e][m] = compose::manager::instance()->create(e);
            spdlog::info("create : {}, {}", e, m);
            compose::model_info mi;
            mi.batch = 1;
            mi.channel = 3;
            mi.height = 224;
            mi.width = 224;
            mi.layout = compose::data_layout::nhwc;

            data[e][m]->init("./model/" + m, mi);
            auto e1 = high_resolution_clock::now();
            spdlog::info("{}, {}, \t {} ns", e, m, (e1 - s1).count());
        }
    }
    auto e = high_resolution_clock::now();

    spdlog::info("fin : {} ns", (e - s).count());

}
