#ifndef MAIN_MALI_INTERFACE_INFERENCE_ENGINE_H
#define MAIN_MALI_INTERFACE_INFERENCE_ENGINE_H

#include <compose/iengine.h>

class mali_engine : public iengine { 
public:
    virtual std::string get_name() const;

    virtual void init(const std::string file, compose::model_info info);
    virtual void inference(const std::string image);
    virtual void deinit();
};

#endif // MAIN_MALI_INTERFACE_INFERENCE_ENGINE_H
