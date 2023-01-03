#include <scheduler/min_assign_scheduler.h>
#include <chrono>

void min_assign_scheduler::deinit() { 

}
void min_assign_scheduler::sequence(std::vector<std::string> model_idx) { 
      auto l = compose::manager::instance()->engine_list();
    auto start = high_resolution_clock::now();
    auto cc = high_resolution_clock::now();

    for (int i = 0, j = 0; ; i++, j++) {
        // 버퍼크기 16미만일떄까지 루프 돌아야지
        auto m = model_idx[i % model_idx.size()];
        if (model_idx[i % model_idx.size()] == "resnet101") {
            this->data[compose::engine::rknn]->enqueue(m);
        }else if (model_idx[i % model_idx.size()] == "resnet50") {
            this->data[compose::engine::myriad]->enqueue(m);
        }else {
            this->data[compose::engine::coral]->enqueue(m);
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

        if (j >= 3000) { 
            break;
        }
    }
    printf("finish : %.3f\n\n", (double)(high_resolution_clock::now() - start).count() / 1000 / 1000 / 1000);
}
