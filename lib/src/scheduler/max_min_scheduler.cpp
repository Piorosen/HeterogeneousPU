#include <scheduler/max_min_scheduler.h>

#include <chrono>

using namespace std;
using namespace std::chrono;

void max_min_scheduler::deinit() { 

}

// Model / PU
array<array<double, 3>, 4> inference_table {{

    { 86.3555, 400.072, 888887878 },
    {  999999999,  999999999,   40 },
    {   3,  200,  70.260 },
    {  7.499,  15.345,   31.70 }
}};


array<double, 4> score { 0,0,0,0 };

int get_min_score_idx(array<double, 4> s) { 
    int idx = 0;
    double max_value = -1;

    for (int i = 0; i < s.size(); i++) { 
        max_value = max(max_value, s[i]);
    }
    for (int i = 0; i < s.size(); i++) { 
        if (max_value > s[i]) { 
            max_value = s[i];
            idx = i;
        }
    }
    return idx;
}

void max_min_scheduler::sequence(std::vector<std::string> model_idx) { 
    auto mm_clock = high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        array<double, 4> t_score { 0,0,0,0};
        for (int d = 0; d < 4; d++) { 
            t_score[d] = inference_table[d][2] + score[d];
        }
        int idx = get_min_score_idx(t_score);
        score[idx] += inference_table[idx][2];
        // inferdata[idx].push(2);
        inferdata.push(make_tuple(idx, "resnet101"));
    }
    for (int i = 0; i < 1000; i++) {
        array<double, 4> t_score { 0,0,0,0};
        for (int d = 0; d < 4; d++) { 
            t_score[d] = inference_table[d][1] + score[d];
        }
        int idx = get_min_score_idx(t_score);
        score[idx] += inference_table[idx][1];
        // inferdata[idx].push(1);
        inferdata.push(make_tuple(idx, "resnet50"));
    }
    for (int i = 0; i < 1000; i++) {
        array<double, 4> t_score { 0,0,0,0};
        for (int d = 0; d < 4; d++) { 
            t_score[d] = inference_table[d][0] + score[d];
        }
        int idx = get_min_score_idx(t_score);
        score[idx] += inference_table[idx][0];
        // inferdata[idx].push(1);
        inferdata.push(make_tuple(idx, "mobilenet"));
    }
    printf("init : %f\n\n", (high_resolution_clock::now() - mm_clock).count() / 1000.0 / 1000.0);

    auto l = compose::manager::instance()->engine_list();
    auto start = high_resolution_clock::now();
    auto cc = high_resolution_clock::now();
     for (int i = 0, j = 0; true; i++, j++) {
        // 버퍼크기 16미만일떄까지 루프 돌아야지
        auto m = model_idx[i % model_idx.size()];

        if (!inferdata.empty()) { 
            compose::engine e;
            auto f = inferdata.front();

            inferdata.pop();
            if (get<0>(f) == 0) {       // CPU
                e = compose::engine::neon;
            }else if (get<0>(f) == 1) { // NCS2
                e = compose::engine::myriad;
            }else if (get<0>(f) == 2) { // Coral
                e = compose::engine::coral;
            }else if (get<0>(f) == 3) { // Rockchip
                e = compose::engine::rknn;
            }

            this->data[e]->enqueue(get<1>(f));
        }
        
        

        // printf("%d\n",  (high_resolution_clock::now() - cc).count());
        if ((high_resolution_clock::now() - cc).count() / 1000 / 1000 > 3000) { 
            int s = 0;
            for (const auto& engine : this->data) { 
                printf("%s : %.3f \tfps\n", compose::engine_to_string(engine.first).c_str(), engine.second->get_fps());
                s += engine.second->get_compute();
            }
            if (s > 2950) { 
                break;
            }
            printf("total  : %.3f \tfps\n", ((double)i) / ((double)(high_resolution_clock::now() - cc).count() / 1000 / 1000 / 1000));
            printf("start : %.3f\n\n", (double)(high_resolution_clock::now() - start).count() / 1000 / 1000 / 1000);
            
            cc = high_resolution_clock::now();
            i = 0;
        }
    }
    printf("finish : %.3f\n\n", (double)(high_resolution_clock::now() - start).count() / 1000 / 1000 / 1000);

}
