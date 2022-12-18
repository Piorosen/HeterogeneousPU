#ifndef MAIN_RKNN_INTERFACE_INFERENCE_ENGINE_H
#define MAIN_RKNN_INTERFACE_INFERENCE_ENGINE_H

#include <compose/iengine.h>
#include <SimpleRKNN/simple_rknn.h>

class rknn_engine : public iengine { 
private:
    rknn::simple_rknn lib;

public:
    virtual std::string get_name() const;

    virtual void init(const std::string file);
    virtual void inference(const std::string image);
    virtual void deinit();
};

#endif // MAIN_RKNN_INTERFACE_INFERENCE_ENGINE_H
