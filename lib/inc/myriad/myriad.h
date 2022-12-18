#ifndef MAIN_MYRIAD_INTERFACE_INFERENCE_ENGINE_H
#define MAIN_MYRIAD_INTERFACE_INFERENCE_ENGINE_H

#include <compose/iengine.h>
#include <openvino/openvino.hpp>

class myriad_engine : public iengine { 
private:
    ov::CompiledModel compiled_model;
    ov::Shape input_shape;

public:
    virtual std::string get_name() const;

    virtual void init(const std::string file);
    virtual void inference(const std::string image);
    virtual void deinit();
};

#endif // MAIN_MYRIAD_INTERFACE_INFERENCE_ENGINE_H
