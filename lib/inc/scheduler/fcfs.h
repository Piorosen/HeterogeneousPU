#pragma once

#include <scheduler/ischeduler.h>

class fcfs : public ischeduler { 
public:
    virtual std::string get_name() const { return "fcfs"; }
    virtual void init(const std::string file) {}
    virtual void deinit() {}
    virtual void sequence(const std::string file) {}
};
