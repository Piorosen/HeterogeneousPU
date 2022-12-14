#ifndef MAIN_EDGETPU_INTERFACE_INFERENCE_ENGINE_H
#define MAIN_EDGETPU_INTERFACE_INFERENCE_ENGINE_H

#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/kernels/register.h>

#include <edgetpu.h>
#include <edgetpu_c.h>

#include <compose/iengine.h>
#include <memory>

class edgetpu_engine : public iengine { 
private:
    std::unique_ptr<tflite::Interpreter> interpreter;
    std::shared_ptr<tflite::FlatBufferModel> model;
    // tflite::ops::builtin::BuiltinOpResolver resolver;

public:
    virtual std::string get_name() const;

    virtual void init(const std::string file, compose::model_info info) override;
    virtual void inference(const std::string image) override;
    virtual void deinit() override;
};

#endif // MAIN_EDGETPU_INTERFACE_INFERENCE_ENGINE_H
