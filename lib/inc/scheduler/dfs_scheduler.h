#pragma once

#include <scheduler/ischeduler.h>

class dfs_scheduler : public ischeduler { 
public:
    virtual std::string get_name() const { return "dfs_scheduler"; }
    virtual void deinit() override;
    virtual void sequence(std::vector<std::string> model_idx) override;
};
