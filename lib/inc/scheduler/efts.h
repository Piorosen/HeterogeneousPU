#pragma once

#include <scheduler/ischeduler.h>

class efts : public ischeduler { 
public:
    virtual std::string get_name() const { return "etfs"; }
    virtual void deinit() override;
    virtual void sequence(std::vector<std::string> model_idx) override;
};
