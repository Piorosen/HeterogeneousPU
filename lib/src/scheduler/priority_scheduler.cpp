
#include <scheduler/priority_scheduler.h>

#include <chrono>

using namespace std;
using namespace std::chrono;

void priority_scheduler::deinit() { 

}

void priority_scheduler::sequence(std::vector<std::string> model_idx) { 
    auto l = compose::manager::instance()->engine_list();
    auto start = high_resolution_clock::now();
    auto cc = high_resolution_clock::now();
     for (int i = 0, j = 0; true; i++, j++) {
        // 버퍼크기 16미만일떄까지 루프 돌아야지
        auto m = model_idx[i % model_idx.size()];

        // if (!inferdata.empty()) { 
        //     compose::engine e;
        //     auto f = inferdata.front();

        //     inferdata.pop();
        //     if (get<0>(f) == 0) {       // CPU
        //         e = compose::engine::neon;
        //     }else if (get<0>(f) == 1) { // NCS2
        //         e = compose::engine::myriad;
        //     }else if (get<0>(f) == 2) { // Coral
        //         e = compose::engine::coral;
        //     }else if (get<0>(f) == 3) { // Rockchip
        //         e = compose::engine::rknn;
        //     }

        //     this->data[e]->enqueue(get<1>(f));
        // }
        
        
        // printf("%d\n",  (high_resolution_clock::now() - cc).count());
        if ((high_resolution_clock::now() - cc).count() / 1000 / 1000 > 3000) { 
            for (const auto& engine : this->data) { 
                printf("%s : %.3f \tfps\n", compose::engine_to_string(engine.first).c_str(), engine.second->get_fps());
            }
            printf("total  : %.3f \tfps\n", ((double)i) / ((double)(high_resolution_clock::now() - cc).count() / 1000 / 1000 / 1000));
            printf("start : %.3f\n\n", (double)(high_resolution_clock::now() - start).count() / 1000 / 1000 / 1000);
            
            cc = high_resolution_clock::now();
            i = 0;
        }

        int s = 0;
        for (const auto& engine : this->data) { 
            s += engine.second->get_compute();
        }
        if (s >= 3000) { 
            break;
        }
     }
    printf("finish : %.3f\n\n", (double)(high_resolution_clock::now() - start).count() / 1000 / 1000 / 1000);

    for (const auto& engine : this->data) { 
        engine.second->inference_result();
    }
}
