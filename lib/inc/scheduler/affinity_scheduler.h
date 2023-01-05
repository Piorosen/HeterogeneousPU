#pragma once

#include <scheduler/ischeduler.h>
#include <string>
#include <array>
#include <queue>
#include <tuple>

using namespace std;

class affinity_scheduler : public ischeduler { 
private:
// engine, model
    queue<tuple<int, string>> inferdata;
public:
    virtual std::string get_name() const { return "affinity_scheduler"; }
    virtual void deinit() override;
    virtual void sequence(std::vector<std::string> model_idx) override;
};
