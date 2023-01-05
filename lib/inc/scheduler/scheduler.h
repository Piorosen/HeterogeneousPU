#pragma once

#include <scheduler/first_come_first_served_scheduler.h>
#include <scheduler/affinity_scheduler.h>
#include <scheduler/priority_scheduler.h>
#include <scheduler/short_job_first_scheduler.h>
#include <memory>


class scheduler { 
private:
    first_come_first_served_scheduler fcfs;
    affinity_scheduler as;
    priority_scheduler rtg;
    short_job_first_scheduler ma;
    
public:
    static scheduler* instance() { 
        static scheduler self;
        return &self;
    }


    ischeduler* first_come_first_served() {
        spdlog::info("first_come_first_served_scheduler");
        return &fcfs;
    }
    
    ischeduler* affinity() {
        spdlog::info("affinity_scheduler");
        return &as;
    }

    ischeduler* priority() {
        spdlog::info("priority_scheduler");
        return &rtg;
    }
    
    ischeduler* short_job_first() {
        spdlog::info("short_job_first_scheduler");
        return &ma;
    }
};
