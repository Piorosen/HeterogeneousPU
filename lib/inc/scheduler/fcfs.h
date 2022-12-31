#pragma once

#include <scheduler/ischeduler.h>

class fcfs : public ischeduler { 
public:
    virtual std::string get_name() const { return "fcfs"; }
    virtual void deinit() override;
    virtual void sequence(std::vector<std::string> model_idx) override;
};
