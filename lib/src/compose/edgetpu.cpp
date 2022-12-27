
#include "extension/edgetpu.h"
#include <compose/edgetpu.h>

std::string edgetpu_engine::get_name() const {
    return "edgetpu";
}

void edgetpu_engine::init(const std::string file, compose::model_info info) {
    // Find TPU device.
    size_t num_devices;
    std::unique_ptr<edgetpu_device, decltype(&edgetpu_free_devices)> devices(
        edgetpu_list_devices(&num_devices), &edgetpu_free_devices);

    if (num_devices == 0) {
        std::cerr << "No connected TPU found" << std::endl;
        throw;
    }
    const auto& device = devices.get()[0];

    // // Load labels.
    // auto labels = tpu::ReadLabels(label_file);
    // if (labels.empty()) {
    //     std::cerr << "Cannot read labels from " << label_file << std::endl;
    //     return;
    // }

    // Load model.
    auto model = tflite::FlatBufferModel::BuildFromFile(file.c_str());
    if (!model) {
        std::cerr << "Cannot read model from " << file << std::endl;
        throw;
    }

    // Create interpreter.
    tflite::ops::builtin::BuiltinOpResolver resolver;
    if (tflite::InterpreterBuilder(*model, resolver)(&this->interpreter) != kTfLiteOk) {
        std::cerr << "Cannot create interpreter" << std::endl;
        throw;
    }

    auto* delegate = edgetpu_create_delegate(device.type, device.path, nullptr, 0);
    //
    //   interpreter->ModifyGraphWithDelegate(std::make_shared<TfLiteDelegate, TfLiteDelegate>(delegate, edgetpu_free_delegate));
    //   interpreter->ModifyGraphWithDelegate(delegate);

    // Allocate tensors.
    if (interpreter->AllocateTensors() != kTfLiteOk) {
        std::cerr << "Cannot allocate interpreter tensors" << std::endl;
        throw;
    }

    // Set interpreter input.
    const auto* input_tensor = interpreter->input_tensor(0);
    if (input_tensor->type != kTfLiteUInt8 ||           //
        input_tensor->dims->data[0] != 1 ||             //
        input_tensor->dims->data[1] != info.height ||  //
        input_tensor->dims->data[2] != info.width ||   //
        input_tensor->dims->data[3] != info.channel) {
        std::cerr << "Input tensor shape does not match input image" << std::endl;
        throw;
    }

}
void edgetpu_engine::inference(const std::string image_file) {
    // if (argc != 5) {
    //     std::cerr << argv[0]
    //             << " <model_file> <label_file> <image_file> <threshold>"
    //             << std::endl;
    //     return 1;
    // }
    // const std::string model_file = argv[1];
    // const std::string label_file = argv[2];
    // const std::string image_file = argv[3];
    // const float threshold = std::stof(argv[4]);

    // Load image.
    int image_bpp, image_width, image_height;
    auto image =
        tpu::ReadBmpImage(image_file.c_str(), &image_width, &image_height, &image_bpp);
    if (image.empty()) {
        std::cerr << "Cannot read image from " << image_file << std::endl;
        throw;
    }

    std::copy(image.begin(), image.end(),
                interpreter->typed_input_tensor<uint8_t>(0));

    // Run inference.
    if (interpreter->Invoke() != kTfLiteOk) {
        std::cerr << "Cannot invoke interpreter" << std::endl;
        throw;
    }

    // Get interpreter output.
    // auto results = tpu::Sort(tpu::Dequantize(*interpreter->output_tensor(0)), threshold);
    // for (auto& result : results)
    //     std::cout << std::setw(7) << std::fixed << std::setprecision(5)
    //             << result.second << tpu::GetLabel(labels, result.first) << std::endl;
    return;
}
void edgetpu_engine::deinit() {

}
