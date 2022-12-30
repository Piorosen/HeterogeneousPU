#pragma once

#include <scheduler/efts.h>
#include <scheduler/fcfs.h>
#include <memory>

class scheduler { 
private:
    efts eft;
    fcfs fcf;
    
public:
    static scheduler* instance() { 
        static scheduler self;
        return &self;
    }

    ischeduler* eft_scheduler() {
        return &eft;
    }

    ischeduler* fcf_scheduler() {
        return &fcf;
    }
};
