#pragma once

#include <scheduler/ischeduler.h>

class first_come_first_served_scheduler : public ischeduler { 
public:
    virtual std::string get_name() const { return "first_come_first_served"; }
    virtual void deinit() override;
    virtual void sequence(std::vector<std::string> model_idx) override;
};
