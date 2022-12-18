#include <compose/manager.h>

#include <mali/mali.h>
#include <neon/neon.h>
#include <rknn/rknn.h>
#include <myriad/myriad.h>

std::vector<compose::engine> compose::manager::engine_list() const {
    return std::vector<compose::engine>{
        compose::engine::myriad,
        compose::engine::mali,
        compose::engine::neon,
        compose::engine::rknn,
    };
}

std::vector<std::shared_ptr<iengine>> compose::manager::inference_engine() {
    return std::vector<std::shared_ptr<iengine>> {
        // std::make_shared<mali_engine>(),
        std::make_shared<neon_engine>(),
        std::make_shared<myriad_engine>(),
        std::make_shared<rknn_engine>(),
    };
}
