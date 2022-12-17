#ifndef MAIN_COMPOSE_OPENVINO_H
#define MAIN_COMPOSE_OPENVINO_H

#include <iostream>
// #include <sys/time.h>
// #include <chrono>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <stb/stb_image.h>
#include <openvino/openvino.hpp>
#include "classification_results.h"

using namespace std;
using namespace std::chrono;

void show(const ov::Model* network){
    cout << "model name: " << network->get_friendly_name() << endl;

    const std::vector<ov::Output<const ov::Node>> inputs = network->inputs();
    for (const ov::Output<const ov::Node> input : inputs) {
        cout << "    inputs" << endl;

        const std::string name = input.get_names().empty() ? "NONE" : input.get_any_name();
        cout << "        input name: " << name << endl;

        const ov::element::Type type = input.get_element_type();
        cout << "        input type: " << type << endl;

        const ov::Shape shape = input.get_shape();
        cout << "        input shape: " << shape << endl;
    }

    const std::vector<ov::Output<const ov::Node>> outputs = network->outputs();
    for (const ov::Output<const ov::Node> output : outputs) {
        cout << "    outputs" << endl;

        const std::string name = output.get_names().empty() ? "NONE" : output.get_any_name();
        cout << "        output name: " << name << endl;

        const ov::element::Type type = output.get_element_type();
        cout << "        output type: " << type << endl;

        const ov::Shape shape = output.get_shape();
        cout << "        output shape: " << shape << endl;
    }
}


std::vector<double> run_vino() { 

    auto version = ov::get_openvino_version();
    cout << version.buildNumber << " :-: " << version.description << "\n";
    std::vector<double> measure_bench;
    ov::Core core;
    vector<string> availableDevices = core.get_available_devices();
    cout << "Available Devices : \n";
    for (auto&& device : availableDevices) {
        cout << device << "\n";
    }

    // OPENVINO_ASSERT(model->inputs().size() == 1, "Sample supports models with 1 input only");
    // OPENVINO_ASSERT(model->outputs().size() == 1, "Sample supports models with 1 output only");

    std::shared_ptr<ov::Model> model = core.read_model("./openvino/resnet101.xml");
    // show(model.get());
    int width, height, c;
    unsigned char* data = stbi_load("dog_224x224.jpg", &width, &height, &c, 0);
    ov::element::Type input_type = ov::element::u8;
    ov::Shape input_shape = {1, (unsigned long)height, (unsigned long)width, (unsigned long)c};

    ov::Tensor input_tensor = ov::Tensor(input_type, input_shape, data);

    const ov::Layout tensor_layout{"NHWC"};

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


    // -------- Step 5. Loading a model to the device --------
    ov::CompiledModel compiled_model = core.compile_model(model, availableDevices[0]);

    // -------- Step 6. Create an infer request --------
    ov::InferRequest infer_request = compiled_model.create_infer_request();

    // -------- Step 7. Prepare input --------
    infer_request.set_input_tensor(input_tensor);

    // -------- Step 8. Do inference synchronously --------
    cout << ":: OpenVINO TEST :: " << "\n";
    for (int i = 0; i < 30; i++) {
        auto now = high_resolution_clock::now();
        infer_request.infer();
        cout << (high_resolution_clock::now() - now).count() / 1000 / 1000 << "ms\n";
        measure_bench.push_back((high_resolution_clock::now() - now).count() / 1000 / 1000);
    }
    
    
    // -------- Step 9. Process output
    const ov::Tensor& output_tensor = infer_request.get_output_tensor();

    // Print classification results
    ClassificationResult classification_result(output_tensor, {"dog_224x224.jpg"});
    classification_result.show();

    // spdlog::info("run!!");   
    // cout << arm_compute::Scheduler::get().cpu_info().get_cpu_num() << "\n";
    stbi_image_free(data);
    
    return measure_bench;
}

#endif // MAIN_COMPOSE_OPENVINO_H