#include <scheduler/sjf.h>
#include <chrono>

void sjf::deinit() { 

}
void sjf::sequence(std::vector<std::string> model_idx) { 
    auto l = compose::manager::instance()->engine_list();
    auto start = high_resolution_clock::now();
    auto cc = high_resolution_clock::now();

    for (int i = 0; ; i++) {
        // 버퍼크기 16미만일떄까지 루프 돌아야지
        std::shared_ptr<buf_pu_queue> sel = nullptr;
        auto m = model_idx[i % model_idx.size()];
        double ms = 1000 * 1000 * 1000;

        for (const auto& s : this->data) { 
            if (m != model_idx[0] && s.first == compose::engine::myriad) { 
                continue;
            }
            double get_s = s.second->weight_value(m);
            if (ms < get_s) { 
                sel = s.second;
                ms = get_s;
            }
        }
        if (sel == nullptr) { 
            throw;
        }
        
        sel->enqueue(m);
        // printf("%d\n",  (high_resolution_clock::now() - cc).count());
        if ((high_resolution_clock::now() - cc).count() / 1000 / 1000 > 3000) { 
            for (const auto& engine : this->data) { 
                printf("%s : %.3f \tfps\n", compose::engine_to_string(engine.first).c_str(), engine.second->get_fps());
            }
            printf("total  : %.3f \tfps\n", ((double)200) / ((double)(high_resolution_clock::now() - cc).count() / 1000 / 1000 / 1000));
            printf("start : %.3f\n\n", (double)(high_resolution_clock::now() - start).count() / 1000 / 1000 / 1000);
            
            cc = high_resolution_clock::now();
            i = 0;
        }
    }
}
