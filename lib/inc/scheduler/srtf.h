#pragma once

#include <scheduler/ischeduler.h>

class srtf : public ischeduler { 
public:
    virtual std::string get_name() const { return "srtf"; }
    virtual void deinit();
    virtual void sequence(std::vector<int> model_idx);
};
