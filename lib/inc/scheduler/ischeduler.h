#pragma once

#include <string>
#include <vector>

class ischeduler { 
public:
    virtual std::string get_name() const = 0;
    virtual void init(const std::string file) = 0;
    virtual void sequence(const std::string file) = 0;
    virtual void deinit() = 0;
};