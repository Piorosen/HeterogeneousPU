#ifndef MAIN_RKNN_INTERFACE_INFERENCE_ENGINE_H
#define MAIN_RKNN_INTERFACE_INFERENCE_ENGINE_H

#include <compose/iengine.h>

#ifdef USE_NPU_RKNN
#include <SimpleRKNN/simple_rknn.h>
#endif

class rknn_engine : public iengine { 
private:
#ifdef USE_NPU_RKNN
    rknn::simple_rknn lib;
#endif
    compose::model_info info;
    
public:
    virtual std::string get_name() const;

    virtual void init(const std::string file, compose::model_info info);
    virtual void inference(const std::string image);
    virtual void deinit();
};

#endif // MAIN_RKNN_INTERFACE_INFERENCE_ENGINE_H
