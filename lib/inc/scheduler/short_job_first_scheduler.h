#pragma once

#include <scheduler/ischeduler.h>

class short_job_first_scheduler : public ischeduler { 
public:
    virtual std::string get_name() const { return "short_job_first_scheduler"; }
    virtual void deinit() override;
    virtual void sequence(std::vector<std::string> model_idx) override;
};
