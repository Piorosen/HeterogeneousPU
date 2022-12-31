#include <compose/rknn.h>
#include <vector>
#include <chrono>
#include <array>

void* img = nullptr;

std::string rknn_engine::get_name() const { 
    return "rknn";
}

void rknn_engine::init(const std::string file, compose::model_info info) { 
    printf("rknnn load : %s start\n", file.c_str());
    #ifdef USE_NPU_RKNN
    this->info = info;
    lib.load_model("./" + file + "/rknn/saved_model.rknn");
    // lib.load_model("./mobilenet_v1.rknn");
    #endif
    printf("rknnn load : %s fin\n", file.c_str());
}

void rknn_engine::inference(const std::string image) {
    this->is_inference = true;
    #ifdef USE_NPU_RKNN
    rknn::tensor_format layout;
    switch (info.layout) { 
        case compose::data_layout::nchw:
            layout = rknn::tensor_format::nchw;
            break;
        case compose::data_layout::nhwc:
            layout = rknn::tensor_format::nhwc;
            break;
    }

    if (img == nullptr) { 
        img = lib.load_image(image.c_str(), layout);
    }

    volatile bool flag = false;
    
    lib.compute(img, layout, rknn::tensor_type::uint8, 0, [&flag](auto buf, auto size) {
        flag = true;
    });

    while(!flag);
    #endif
    this->is_inference = false;
}

void rknn_engine::deinit() { 
    #ifdef USE_NPU_RKNN
    rknn::close_loop();
    #endif
}
