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
        return &eft;
    }

    ischeduler* fcfs_scheduler() {
        return &fcf;
    }
    
    ischeduler* sjf_scheduler() {
        return &sj;
    }

    ischeduler* srtf_scheduler() {
        return &srt;
    }
};
