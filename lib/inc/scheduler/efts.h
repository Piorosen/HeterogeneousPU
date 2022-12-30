#pragma once

#include <scheduler/ischeduler.h>

class efts : public ischeduler { 
public:
    virtual std::string get_name() const { return "etfs"; }
    virtual void init(const std::string file) {}
    virtual void deinit() {}
    virtual void sequence(const std::string file) {}
};
