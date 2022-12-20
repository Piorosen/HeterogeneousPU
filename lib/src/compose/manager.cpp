#include <compose/manager.h>

#include <compose/mali.h>
#include <compose/neon.h>
#include <compose/rknn.h>
#include <compose/myriad.h>

std::vector<compose::engine> compose::manager::engine_list() const {
    return std::vector<compose::engine>{
#ifdef USE_NPU_MYRIAD
        compose::engine::myriad,
#endif
#ifdef USE_GPU_MALI
        compose::engine::mali,
#endif
#ifdef USE_CPU_ARM
        compose::engine::neon,
#endif
#ifdef USE_NPU_RKNN
        compose::engine::rknn,
#endif

    };
}

std::vector<std::shared_ptr<iengine>> compose::manager::inference_engine() {
    return std::vector<std::shared_ptr<iengine>> {
#ifdef USE_GPU_MALI
        std::make_shared<mali_engine>(),
#endif
#if USE_NPU_RKNN
        std::make_shared<rknn_engine>(),
#endif
#ifdef USE_CPU_ARM
        std::make_shared<neon_engine>(),
#endif
#ifdef USE_NPU_MYRIAD
        std::make_shared<myriad_engine>(),
#endif
    };
}
