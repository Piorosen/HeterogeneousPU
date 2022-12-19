#include <myriad/myriad.h>
#include <stb/stb_image.h>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <iostream>

using namespace std;

std::string myriad_engine::get_name() const { 
    return "myriad";
}

void myriad_engine::init(const std::string file) { 
    auto version = ov::get_openvino_version();
    cout << version.buildNumber << " :-: " << version.description << "\n";
    std::vector<double> measure_bench;
    ov::Core core;
    std::vector<std::string> availableDevices = core.get_available_devices();
    cout << "Available Devices : \n";
    for (auto&& device : availableDevices) {
        cout << device << "\n";
    }
    
    // OPENVINO_ASSERT(model->inputs().size() == 1, "Sample supports models with 1 input only");
    // OPENVINO_ASSERT(model->outputs().size() == 1, "Sample supports models with 1 output only");

    // std::shared_ptr<ov::Model> model = core.read_model("./openvino/resnet101.xml");
    std::shared_ptr<ov::Model> model = core.read_model("./" + file + "/myriad/saved_model.xml");
    // show(model.get());

    ov::element::Type input_type = ov::element::u8;
    const ov::Layout tensor_layout{"NHWC"};
    

    // ov::Shape input_shape = {1, (unsigned long)height, (unsigned long)width, (unsigned long)c};
    input_shape = {1, (unsigned long)227, (unsigned long)227, (unsigned long)3};


    ov::preprocess::PrePostProcessor ppp(model);

    // 1) Set input tensor information:
    // - input() provides information about a single model input
    // - reuse precision and shape from already available `input_tensor`
    // - layout of data is 'NHWC'
    ppp.input().tensor().set_shape(input_shape).set_element_type(input_type).set_layout(tensor_layout);
    // 2) Adding explicit preprocessing steps:
    // - convert layout to 'NCHW' (from 'NHWC' specified above at tensor layout)
    // - apply linear resize from tensor spatial dims to model spatial dims
    ppp.input().preprocess().resize(ov::preprocess::ResizeAlgorithm::RESIZE_LINEAR);
    // 4) Here we suppose model has 'NCHW' layout for input
    ppp.input().model().set_layout("NCHW");
    // 5) Set output tensor information:
    // - precision of tensor is supposed to be 'f32'
    ppp.output().tensor().set_element_type(ov::element::f32);

    // 6) Apply preprocessing modifying the original 'model'
    model = ppp.build();

    compiled_model = core.compile_model(model, availableDevices[0]);
}

unsigned char* data = nullptr;
int width, height, c;

void myriad_engine::inference(const std::string image) {
    ov::InferRequest infer_request = compiled_model.create_infer_request();
    if (data == nullptr) { 
        data = stbi_load(image.c_str(), &width, &height, &c, 0);
    }
    ov::Tensor input_tensor = ov::Tensor(ov::element::u8, this->input_shape, data);
    infer_request.set_input_tensor(input_tensor);
    infer_request.infer();
}

void myriad_engine::deinit() { 
    stbi_image_free(data);
}