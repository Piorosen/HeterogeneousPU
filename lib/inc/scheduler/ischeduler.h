#pragma once

#include <spdlog/spdlog.h>

#include <string>
#include <vector>
#include <map>
#include <compose/manager.h>
#include <memory>

class ischeduler { 
private:
    std::map<compose::engine, std::map<std::string, std::shared_ptr<iengine>>> data;

public:
    virtual std::string get_name() const = 0;
    virtual void init(const std::vector<std::string> models);
    virtual void sequence(std::vector<int> model_idx) = 0;
    virtual void deinit() = 0;
};
