#include <scheduler/round_robine.h>
#include <random>
#include <chrono>

void round_robine::deinit() { 

}

void round_robine::sequence(std::vector<std::string> model_idx) {
    auto l = compose::manager::instance()->engine_list();
    std::random_device rd;
    auto start = high_resolution_clock::now();
    auto cc = high_resolution_clock::now();
    for (int i = 0; ; i++) {
        this->data[l[i % l.size()]]->enqueue(model_idx[i % model_idx.size()]);
        if (i % 200 == 0) { 
            for (const auto& engine : this->data) { 
                printf("%s : %.3f \tfps\n", compose::engine_to_string(engine.first).c_str(), engine.second->get_fps());
            }
            printf("stotal : %.3f \tfps\n", ((double)200) / ((double)(high_resolution_clock::now() - cc).count() / 1000 / 1000 / 1000));
            printf("start : %.3f\n\n", (double)(high_resolution_clock::now() - start).count() / 1000 / 1000 / 1000);
            
            cc = high_resolution_clock::now();
            i = 0;
        }
    }
}
