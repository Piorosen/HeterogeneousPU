#pragma once

#include <scheduler/ischeduler.h>

class srtf : public ischeduler { 
public:
    virtual std::string get_name() const { return "srtf"; }
    virtual void deinit() override;
    virtual void sequence(std::vector<std::string> model_idx) override;
};
