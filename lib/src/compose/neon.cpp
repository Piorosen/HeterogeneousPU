#include <compose/neon.h>

using namespace arm_compute;
using namespace arm_compute::utils;
using namespace arm_compute::graph::frontend;
using namespace arm_compute::graph_utils;

#include "extension/armnn.h"
#include "extension/ObjectDetection.h"
std::string neon_engine::get_name() const { 
    return "neon";
}

void neon_engine::init(const std::string file, compose::model_info info) { 

}

void neon_engine::inference(const std::string image) {

}
void neon_engine::deinit() { 
    delete graph;
}
