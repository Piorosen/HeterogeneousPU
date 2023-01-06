#pragma once

// 동기적으로 순차적으로 처리 할 수 있는 시스템을 구축합니다.
// 싱글톤으로 구현해야하며, 뭐.. 등등? 해야겠죠?
#include <thread>
#include <string>
#include <map>
#include <compose/manager.h>
#include <ctime>

#include <chrono>

#include <msd/channel.hpp>
#include <functional>

using namespace std;
using namespace std::chrono;

class buf_pu_queue { 
private:
    std::thread loop;
    msd::channel<std::string> chan; // buffered
    bool stop_signal = false;
    std::string name = "none";
    int inference_ = 0;
    time_point<system_clock> tt = high_resolution_clock::now();
    std::map<std::string, double> score_model;
    int total_inference = 0;
    
    map<string, int> pu_to_idx;

    buf_pu_queue(const buf_pu_queue& oth) = delete;
public:
    double get_fps() { 
        double r = (double)inference_ / ((double)(high_resolution_clock::now() - tt).count() / 1000 / 1000 / 1000);
        tt = high_resolution_clock::now();
        inference_ = 0;
        return r;
    }
    int get_buf() const { 
        return chan.size();
    }
    buf_pu_queue(int queue_size = 32) : chan(queue_size) { 
        pu_to_idx["mobilenet"] = 0;
        pu_to_idx["resnet50"] = 0;
        pu_to_idx["resnet101"] = 0;
    }

    std::string get_name() const { return name; }
    
    double weight_value(std::string data);
    void enqueue(std::string data);
    
    void run_loop(std::vector<std::string> model, compose::engine e, std::function<void()> callback);
    void close_loop();
    int get_compute() const { 
        return total_inference;
    }
    void inference_result() const  {
        printf("Inference Engine : [ %s ]\n", this->get_name().c_str()); 
        for (const auto& p : pu_to_idx) { 
            printf("%s : %04d\n", p.first.c_str(), p.second);
        }
    }

};
