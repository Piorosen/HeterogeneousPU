#include <rknn/rknn.h>
#include <vector>
#include <chrono>
#include <array>

void* img = nullptr;

std::string rknn_engine::get_name() const { 
    return "rknn";
}

void rknn_engine::init(const std::string file) { 
    rknn::run_loop();
    lib.load_model("./" + file + "/rknn/" + file + ".rknn");
    // lib.load_model("./mobilenet_v1.rknn");
}

void rknn_engine::inference(const std::string image) {
    if (img == nullptr) { 
        img = lib.load_image(image.c_str(), rknn::tensor_format::nchw);
    }

    bool flag = false;
    lib.compute(img, rknn::tensor_format::nchw, rknn::tensor_type::uint8, 0, [&flag](auto buf, auto size) {
        // cout << (high_resolution_clock::now() - compute).count() << "\n";
        flag = true;
    });

    while(!flag);
}

void rknn_engine::deinit() { 
    rknn::close_loop();
}
