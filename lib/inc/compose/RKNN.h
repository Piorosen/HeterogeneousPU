#ifndef MAIN_COMPOSE_RKNNL_H
#define MAIN_COMPOSE_RKNNL_H
#include <vector>
#include <chrono>
#include <array>

#include <SimpleRKNN/simple_rknn.h>

using namespace std;
using namespace std::chrono;

std::vector<double> run_rknn() { 
    rknn::simple_rknn lib;

    rknn::run_loop();

    lib.load_model("mobilenet_v1.rknn");
    auto image = lib.load_image("dog_224x224.jpg", rknn::tensor_format::nchw);

    for (int i = 0; i < 30; i++) { 
        auto compute = high_resolution_clock::now();
        bool flag = false;
        lib.compute(image, rknn::tensor_format::nchw, rknn::tensor_type::uint8, 0, [&lib, &flag, &compute](auto buf, auto size) {
            cout << (high_resolution_clock::now() - compute).count() << "\n";
            flag = true;
        });

        while(!flag);
    }
    
    rknn::close_loop();
    lib.compute(image, rknn::tensor_format::nchw, rknn::tensor_type::uint8, 0, [](auto buf, auto size) {
        });
    lib.free_image(image);
    return vector<double>();
}


#endif // MAIN_COMPOSE_RKNNL_H
