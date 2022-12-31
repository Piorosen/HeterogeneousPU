#pragma once

#include <scheduler/efts.h>
#include <scheduler/fcfs.h>
#include <scheduler/sjf.h>
#include <scheduler/srtf.h>
#include <scheduler/round_robine.h>
#include <scheduler/random_sche.h>
#include <memory>


class scheduler { 
private:
    efts eft;
    fcfs fcf;
    sjf sj;
    srtf srt;
    round_robine rr;
    random_sche rss;
    
    
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
    ischeduler* random_scheduler() {
        spdlog::info("random_sche");
        return &rss;
    }
    ischeduler* round_robine_scheduler() {
        spdlog::info("round_robine_scheduler");
        return &rr;
    }


};
