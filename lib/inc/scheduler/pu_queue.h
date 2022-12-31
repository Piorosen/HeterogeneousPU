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

using namespace std::chrono;

class buf_pu_queue { 
private:
    std::thread loop;
    msd::channel<std::string> chan; // buffered
    bool stop_signal = false;
    std::string name = "none";
    int inference_ = 0;
    time_point<system_clock> tt = high_resolution_clock::now();

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
    buf_pu_queue(int queue_size = 32) : chan(queue_size) { }

    std::string get_name() const { return name; }
    
    void enqueue(std::string data);
    
    void run_loop(std::vector<std::string> model, compose::engine e, std::function<void()> callback);
    void close_loop();
};
