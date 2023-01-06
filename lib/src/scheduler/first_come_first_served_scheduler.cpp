#include <scheduler/first_come_first_served_scheduler.h>
#include <chrono>

using namespace std::chrono;

void first_come_first_served_scheduler::deinit() { 

}

void first_come_first_served_scheduler::sequence(std::vector<std::string> model_idx) {
    auto l = compose::manager::instance()->engine_list();
    auto start = high_resolution_clock::now();
    auto cc = high_resolution_clock::now();

    for (int i = 0, j = 0; ; i++, j++) {
        if (j < 3000) { 
            // 버퍼크기 16미만일떄까지 루프 돌아야지
            std::shared_ptr<buf_pu_queue> sel = nullptr;
            auto m = model_idx[i % model_idx.size()];
            while (true) { 
                for (const auto& s : this->data) { 
                    if (m != model_idx[2] && s.first == compose::engine::myriad) { 
                        continue;
                    }
                    if (s.second->get_buf() < 1) { 
                        sel = s.second;
                    }
                }
                if (sel != nullptr) { 
                    break;
                }
            }
            sel->enqueue(m);
        }

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
