#pragma once

#include <scheduler/fcfs.h>
#include <scheduler/round_robine.h>
#include <memory>


class scheduler { 
private:
    fcfs fcf;
    round_robine rr;
    
public:
    static scheduler* instance() { 
        static scheduler self;
        return &self;
    }


    ischeduler* fcfs_scheduler() {
        spdlog::info("fcfs_scheduler");
        return &fcf;
    }
    
    ischeduler* round_robine_scheduler() {
        spdlog::info("round_robine_scheduler");
        return &rr;
    }


};
