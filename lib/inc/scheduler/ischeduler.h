#pragma once

#include <spdlog/spdlog.h>

#include <string>
#include <vector>
#include <map>
#include <memory>

#include <compose/manager.h>
#include <scheduler/pu_queue.h>

class ischeduler { 
protected:
    std::map<compose::engine, std::shared_ptr<buf_pu_queue>> data;
    int inference_count = 3000;
    int text_out = 3000;

    double compute_fps() {
        double result = 0;
        for (const auto& e : data) { 
            result += e.second->get_fps();
        }
        return result;
    }
public:
    virtual std::string get_name() const = 0;
    virtual void init(const std::vector<std::string> models, int queue_size = 32);
    virtual void sequence(std::vector<std::string> model_idx) = 0;
    virtual void deinit() = 0;

    virtual void set_option(int inference, int text) { 
        this->inference_count = inference;
        this->text_out = text;
    }
};
