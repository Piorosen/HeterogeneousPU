#ifndef MAIN_COMPOSE_INTERFACE_INFERENCE_ENGINE_H
#define MAIN_COMPOSE_INTERFACE_INFERENCE_ENGINE_H

#include <string>
#include <compose/structure.h>

class iengine {
protected:
    bool is_inference = false;

public:
    virtual std::string get_name() const = 0;
    virtual void init(const std::string file, compose::model_info info) = 0;
    virtual void inference(const std::string image) = 0;
    virtual void deinit() = 0;
    virtual bool isInferencing() const { return is_inference; }

};

#endif // MAIN_COMPOSE_INTERFACE_INFERENCE_ENGINE_H
