#pragma once

#include <scheduler/ischeduler.h>

class efts : public ischeduler { 
public:
    virtual std::string get_name() const { return "etfs"; }
    virtual void deinit();
    virtual void sequence(std::vector<int> model_idx);
};
