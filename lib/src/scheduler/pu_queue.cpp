#include <scheduler/pu_queue.h>
#include <chrono>

#ifdef USE_NPU_RKNN
#include <SimpleRKNN/simple_rknn.h>
#endif

using namespace std::chrono;

double buf_pu_queue::weight_value(std::string data) { 
    return this->score_model[data];
}

void buf_pu_queue::run_loop(std::vector<std::string> model, compose::engine e, std::function<void()> callback) { 
    for (const auto& m : model) { 
        this->score_model[m] = 1000 * 100;
        printf("create : %s\n", m.c_str());
    }
    printf("aaa~!~!");
    fflush(stdout);

    loop = std::thread([this, model, e, callback]() { 
        std::map<std::string, std::shared_ptr<iengine>> models;
        for (const auto& m : model) { 
            models[m] = compose::manager::instance()->create(e);

            compose::model_info mi;
            mi.batch = 1;
            mi.channel = 3;
            mi.height = 224;
            mi.width = 224;
            mi.layout = compose::data_layout::nhwc;
            models[m]->init("./model/" + m, mi);
            if (e == compose::engine::myriad) {
                break;
            } 
        }
        #ifdef USE_NPU_RKNN
        if (e == compose::engine::rknn) { 
            rknn::run_loop();
        }
        #endif
        printf("!!!");
        callback();
        int infer = 0;
        
        auto n = clock();

        for (const auto& model : this->chan){
            
            this->inference_ += 1;
            infer += 1;
            if (this->stop_signal == true) { 
                return;
            }
            auto start = high_resolution_clock::now();
            // printf("실행됨! [ %s ] : [ %03d ] [ %s ] \n ", name.c_str(), infer, model.c_str());
            models[model]->inference("00374.jpg");
            this->score_model[model] = (double)(high_resolution_clock::now() - start).count() / 1000 / 1000;
             
        }
    });
}

void buf_pu_queue::close_loop() { 
    stop_signal = true;
}

 void buf_pu_queue::enqueue(std::string data) { 
    data >> this->chan;
 }