#include <scheduler/fcfs.h>
#include <random>

void fcfs::deinit() { 

}

void fcfs::sequence(std::vector<std::string> model_idx) {
    auto l = compose::manager::instance()->engine_list();
    std::random_device rd;

    for (const auto& m : model_idx) {
        printf("%s : models\n", m.c_str());
        compose::engine e;
        std::string mo;

        for (int i = 0; ; i++) {
            this->data[l[rd() % l.size()]]->enqueue(m);
            if (i % 200 == 0) { 
                for (const auto& engine : this->data) { 
                    printf("%s : %.3f \tfps\n", engine.second->get_name(), engine.second->get_fps());
                }
                printf("total : %.3f \tfps\n\n", compute_fps());
            }
        }
    }
}
