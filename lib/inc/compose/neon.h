#ifndef MAIN_NEON_INTERFACE_INFERENCE_ENGINE_H
#define MAIN_NEON_INTERFACE_INFERENCE_ENGINE_H

#include <compose/iengine.h>
#include <compose/extension/ObjectDetection.h>

class neon_engine : public iengine { 
private:
    od::IPipelinePtr target;

public:
    virtual std::string get_name() const;

    virtual void init(const std::string file, compose::model_info info);
    virtual void inference(const std::string image);
    virtual void deinit();
};

#endif // MAIN_NEON_INTERFACE_INFERENCE_ENGINE_H
