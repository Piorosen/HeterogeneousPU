#pragma once

// 동기적으로 순차적으로 처리 할 수 있는 시스템을 구축합니다.
// 싱글톤으로 구현해야하며, 뭐.. 등등? 해야겠죠?
#include <thread>
#include <string>
#include <map>
#include <compose/manager.h>

#include <msd/channel.hpp>
#include <functional>

class buf_pu_queue { 
private:
    std::thread loop;
    msd::channel<std::string> chan{32}; // buffered
    bool stop_signal = false;
    std::string name = "none";
    double fps = 0.0;
    buf_pu_queue(const buf_pu_queue& oth) = delete;

public:
    double get_fps() const { return fps; }

    buf_pu_queue() { }

    std::string get_name() const { return name; }
    
    void enqueue(std::string data);
    
    void run_loop(std::vector<std::string> model, compose::engine e, std::function<void()> callback);
    void close_loop();
};
