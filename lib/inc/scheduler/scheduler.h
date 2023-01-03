#pragma once

#include <scheduler/first_come_first_served_scheduler.h>
#include <scheduler/max_min_scheduler.h>
#include <scheduler/min_assign_scheduler.h>
#include <scheduler/real_time_genetic_scheduler.h>
#include <memory>


class scheduler { 
private:
    first_come_first_served_scheduler fcfs;
    max_min_scheduler mm;
    min_assign_scheduler ma;
    real_time_genetic_scheduler rtg;
    
public:
    static scheduler* instance() { 
        static scheduler self;
        return &self;
    }


    ischeduler* first_come_first_served() {
        spdlog::info("fcfs_scheduler");
        return &fcfs;
    }
    
    ischeduler* max_min() {
        spdlog::info("max_min_scheduler");
        return &mm;
    }

    ischeduler* min_assign() {
        spdlog::info("min_assign");
        return &ma;
    }
    
    ischeduler* real_time_genetic() {
        spdlog::info("real_time_genetic");
        return &rtg;
    }
};
