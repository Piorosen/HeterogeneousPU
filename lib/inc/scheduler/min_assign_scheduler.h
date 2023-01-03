#pragma once

#include <scheduler/ischeduler.h>

class min_assign_scheduler : public ischeduler { 
public:
    virtual std::string get_name() const { return "min_assign"; }
    virtual void deinit() override;
    virtual void sequence(std::vector<std::string> model_idx) override;
};
