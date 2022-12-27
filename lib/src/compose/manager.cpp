#include <compose/manager.h>


#ifdef USE_GPU_MALI
#include <compose/mali.h>
#endif
#ifdef USE_CPU_ARM
#include <compose/neon.h>
#endif
#ifdef USE_NPU_RKNN
#include <compose/rknn.h>
#endif
#ifdef USE_NPU_MYRIAD
#include <compose/myriad.h>
#endif
#ifdef USE_NPU_CORAL
#include <compose/edgetpu.h>
#endif

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
#ifdef USE_NPU_CORAL
        compose::engine::coral,
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
#ifdef USE_NPU_CORAL
        std::make_shared<edgetpu_engine>(),
#endif
    };
}
