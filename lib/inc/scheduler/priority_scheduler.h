#pragma once

#include <scheduler/ischeduler.h>

class priority_scheduler : public ischeduler { 
public:
    virtual std::string get_name() const { return "priority_scheduler"; }
    virtual void deinit() override;
    virtual void sequence(std::vector<std::string> model_idx) override;
};
