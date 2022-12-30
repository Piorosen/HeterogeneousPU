#pragma once

#include <scheduler/ischeduler.h>

class fcfs : public ischeduler { 
public:
    virtual std::string get_name() const { return "fcfs"; }
    virtual void deinit();
    virtual void sequence(std::vector<int> model_idx);
};
