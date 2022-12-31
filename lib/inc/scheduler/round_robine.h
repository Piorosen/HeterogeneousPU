#pragma once

#include <scheduler/ischeduler.h>

class round_robine : public ischeduler { 
public:
    virtual std::string get_name() const { return "round_robin"; }
    virtual void deinit() override;
    virtual void sequence(std::vector<std::string> model_idx) override;
};
