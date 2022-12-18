#ifndef MAIN_COMPOSE_RKNNL_H
#define MAIN_COMPOSE_RKNNL_H
#include <vector>
#include <chrono>
#include <array>
#include <cstdlib>
#include <stb/stb_image.h>

#include <SimpleRKNN/simple_rknn.h>
#include <SimpleRKNN/option.h>
#include <SimpleRKNN/rknn_queue.h>

using namespace std;
using namespace std::chrono;


static unsigned char *load_image(const char *image_path, tensor_format layout, std::array<uint32_t, 3> dims)
{
    int req_height = 0;
    int req_width = 0;
    int req_channel = 0;

    switch (layout)
    {
    case tensor_format::nhwc:
        req_height = dims[2];
        req_width = dims[1];
        req_channel = dims[0];
        break;
    case tensor_format::nchw:
        req_height = dims[1];
        req_width = dims[0];
        req_channel = dims[2];
        break;
    default:
        printf("meet unsupported layout\n");
        return NULL;
    }

    printf("w=%d,h=%d,c=%d, fmt=%d\n", req_width, req_height, req_channel, layout);

    int height = 0;
    int width = 0;
    int channel = 0;

    unsigned char *image_data = stbi_load(image_path, &width, &height, &channel, req_channel);
    if (image_data == NULL)
    {
        printf("load image failed!\n");
        return NULL;
    }

    return image_data;
}

std::vector<double> run_rknn() { 
    simple_rknn lib;

    rknn_queue::instance()->run_loop();
    lib.load_model("mobilenet_v1.rknn");
    auto info = lib.get_info();
    
    
    auto p = load_image("dog_224x224.jpg", tensor_format::nchw, 
            {   info.input[0].dims[0], 
                info.input[0].dims[1],
                info.input[0].dims[2]
            });
    auto compute = high_resolution_clock::now();

    bool flag = false;
    lib.compute((void*)p, tensor_format::nchw, tensor_type::uint8, 0, [&lib, &flag, &compute](auto buf, auto size) {
        flag = true;
        auto end = high_resolution_clock::now();
        cout << (end - compute).count() << "\n";
    });

    while(!flag) { 
        printf("delay\n");
    }

    return vector<double>();
}


#endif // MAIN_COMPOSE_RKNNL_H
