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

std::string compose::engine_to_string(compose::engine e) { 
    switch (e) { 
    case compose::myriad:
        return "myriad";
    case compose::neon:
        return "neon ";
    case compose::rknn:
        return "rknn ";
    case compose::mali:
        return "mali ";
    case compose::coral:
        return "coral";
    default:
        return "error";
    }
}


std::vector<compose::engine> compose::manager::engine_list() const {
    return std::vector<compose::engine>{
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
#ifdef USE_NPU_MYRIAD
        compose::engine::myriad,
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

std::shared_ptr<iengine> compose::manager::create(compose::engine e) const { 
    switch (e) {
#ifdef USE_NPU_MYRIAD
        case compose::engine::myriad:
        printf("create myriad\n");
            return std::make_shared<myriad_engine>();
#endif
#ifdef USE_GPU_MALI
        case compose::engine::mali:
        printf("create mali\n");
            return std::make_shared<mali_engine>();
#endif
#ifdef USE_CPU_ARM
        case compose::engine::neon:
        printf("create neon\n");
            return std::make_shared<neon_engine>();
#endif
#ifdef USE_NPU_RKNN
        case compose::engine::rknn:
        printf("create rknn\n");
            return std::make_shared<rknn_engine>();
#endif
#ifdef USE_NPU_CORAL
        case compose::engine::coral:
                printf("create coral\n");
            return std::make_shared<edgetpu_engine>();
#endif
        default:
            throw "error!!!";
    }
}

