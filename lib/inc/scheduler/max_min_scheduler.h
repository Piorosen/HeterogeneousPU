#pragma once

#include <scheduler/ischeduler.h>

class max_min_scheduler : public ischeduler { 
public:
    virtual std::string get_name() const { return "max_min"; }
    virtual void deinit() override;
    virtual void sequence(std::vector<std::string> model_idx) override;
};
