#pragma once

#include <scheduler/efts.h>
#include <scheduler/fcfs.h>
#include <scheduler/sjf.h>
#include <scheduler/srtf.h>
#include <memory>


class scheduler { 
private:
    efts eft;
    fcfs fcf;
    sjf sj;
    srtf srt;
    
public:
    static scheduler* instance() { 
        static scheduler self;
        return &self;
    }

    ischeduler* efts_scheduler() {
        spdlog::info("efts_scheduler");
        return &eft;
    }

    ischeduler* fcfs_scheduler() {
        spdlog::info("fcfs_scheduler");
        return &fcf;
    }
    
    ischeduler* sjf_scheduler() {
        spdlog::info("sjf_scheduler");
        return &sj;
    }

    ischeduler* srtf_scheduler() {
        spdlog::info("srtf_scheduler");
        return &srt;
    }
};
