#include <compose/rknn.h>
#include <vector>
#include <chrono>
#include <array>

void* img = nullptr;

std::string rknn_engine::get_name() const { 
    return "rknn";
}

void rknn_engine::init(const std::string file, compose::model_info info) { 
    this->info = info;
    rknn::run_loop();
    lib.load_model("./" + file + "/rknn/" + file + ".rknn");
    // lib.load_model("./mobilenet_v1.rknn");
}

void rknn_engine::inference(const std::string image) {
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
}

void rknn_engine::deinit() { 
    rknn::close_loop();
}
