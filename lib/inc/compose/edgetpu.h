#ifndef MAIN_EDGETPU_INTERFACE_INFERENCE_ENGINE_H
#define MAIN_EDGETPU_INTERFACE_INFERENCE_ENGINE_H

#include <compose/iengine.h>
#include <tensorflow/lite/interpreter.h>

class edgetpu_engine : public iengine { 
private:
    std::unique_ptr<tflite::Interpreter> interpreter;

public:
    virtual std::string get_name() const;

    virtual void init(const std::string file, compose::model_info info);
    virtual void inference(const std::string image);
    virtual void deinit();
};

#endif // MAIN_EDGETPU_INTERFACE_INFERENCE_ENGINE_H
