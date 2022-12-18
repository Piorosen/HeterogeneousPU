#ifndef MAIN_NEON_INTERFACE_INFERENCE_ENGINE_H
#define MAIN_NEON_INTERFACE_INFERENCE_ENGINE_H

#include <compose/iengine.h>
#include <arm_compute/graph.h>
#include <support/ToolchainSupport.h>
#include <utils/GraphUtils.h>


using namespace arm_compute;
using namespace arm_compute::utils;
using namespace arm_compute::graph::frontend;
using namespace arm_compute::graph_utils;



class neon_engine : public iengine { 
private:
    // Stream graph;

public:
    virtual std::string get_name() const;

    virtual void init(const std::string file);
    virtual void inference(const std::string image);
    virtual void deinit();
};

#endif // MAIN_NEON_INTERFACE_INFERENCE_ENGINE_H
