#pragma once

#include <string>
#include <vector>

class ischeduler { 
public:
    virtual std::string get_name() const = 0;
    virtual void init(const std::vector<std::string> models) = 0;
    virtual void sequence(std::vector<int> model_idx) = 0;
    virtual void deinit() = 0;
};