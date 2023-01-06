
#include <scheduler/priority_scheduler.h>

#include <chrono>
#include <map>
#include <memory>
#include <queue>
#include <array>
#include <string>

using namespace std;
using namespace std::chrono;

void priority_scheduler::deinit() { 

}

map<string, int> pu_to_idx;

array<array<int, 3>, 4> priority_table {{
    { 0,2,1},
    // { 1,2,0},
    {2,-1,-1},
    {0,2,1},
    {1,0,2}
}};

array<string, 3> model {{
    "mobilenet", "resnet50", "resnet101"
}};

void priority_scheduler::sequence(std::vector<std::string> model_idx) { 
    pu_to_idx["neon"] = 0;
    pu_to_idx["myriad"] = 1;
    pu_to_idx["edgetpu"] = 2;
    pu_to_idx["rknn"] = 3;
    
    auto l = compose::manager::instance()->engine_list();
    auto start = high_resolution_clock::now();
    auto cc = high_resolution_clock::now();

    array<int, 3> count = {{0,0,0}};  

    for (int i = 0, j = 0; true; i++, j++) {
        // 버퍼크기 16미만일떄까지 루프 돌아야지
        if (j < this->inference_count) { 
            // 버퍼크기 16미만일떄까지 루프 돌아야지
            std::shared_ptr<buf_pu_queue> sel = nullptr;
            while (true) { 
                for (const auto& s : this->data) { 
                    if (s.second->get_buf() < 1) { 
                        sel = s.second;
                    }
                }
                if (sel != nullptr) { 
                    break;
                }
            }
            bool run = false;
            for (int a = 0; a < 3; a++) { 
                if (count[priority_table[pu_to_idx[sel->get_name()]][a]] >= 1000) {
                    continue;
                }
                if (priority_table[pu_to_idx[sel->get_name()]][a] == -1) { 
                    continue;
                }
                run = true;
                count[priority_table[pu_to_idx[sel->get_name()]][a]] += 1;
                sel->enqueue(model[priority_table[pu_to_idx[sel->get_name()]][a]]);
                break;
            }
            if (!run) { 
                j -= 1;
            }
        }
        
        
        // printf("%d\n",  (high_resolution_clock::now() - cc).count());
        if ((high_resolution_clock::now() - cc).count() / 1000 / 1000 > this->text_out) { 
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
        // int sss = 0;
        // for (int a = 0; a < 3; a++) { 
        //     sss += count[a];
        // }
        
        // printf("%04d ::: %04d\n", s, sss);
        if (s >= this->inference_count) { 
            break;
        }
     }
    printf("finish : %.3f\n\n", (double)(high_resolution_clock::now() - start).count() / 1000 / 1000 / 1000);

    for (const auto& engine : this->data) { 
        engine.second->inference_result();
    }
}
