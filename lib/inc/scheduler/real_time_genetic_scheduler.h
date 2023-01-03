#pragma once

#include <scheduler/ischeduler.h>

class real_time_genetic_scheduler : public ischeduler { 
public:
    virtual std::string get_name() const { return "real_time_genetic"; }
    virtual void deinit() override;
    virtual void sequence(std::vector<std::string> model_idx) override;
};
