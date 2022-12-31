#pragma once

#include <scheduler/ischeduler.h>

class random_sche : public ischeduler { 
public:
    virtual std::string get_name() const { return "random_sche"; }
    virtual void deinit() override;
    virtual void sequence(std::vector<std::string> model_idx) override;
};
