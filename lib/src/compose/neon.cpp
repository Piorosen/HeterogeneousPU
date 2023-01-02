#include <compose/extension/edgetpu.h>
#include <compose/neon.h>
#include <stb/stb_image.h>

using namespace std;

std::string neon_engine::get_name() const { 
    return "neon";
}

void neon_engine::init(const std::string file, compose::model_info info) {
       // Find TPU device.
    size_t num_devices;
    std::unique_ptr<edgetpu_device, decltype(&edgetpu_free_devices)> devices(
        edgetpu_list_devices(&num_devices), &edgetpu_free_devices);

    if (num_devices == 0) {
        std::cerr << "No connected TPU found" << std::endl;
        throw;
    }
    printf("1\n");

    const auto& device = devices.get()[0];
    for (int i = 0; i < num_devices; i++) { 
        auto& d = devices.get()[i];
        printf("%d %d, %s\n", i, d.type, d.path);
    }

    model = tflite::FlatBufferModel::BuildFromFile(("./" + file + "/tflite/cpu_saved_model.tflite").c_str());
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
    interpreter->ModifyGraphWithDelegate(delegate);

    // // Allocate tensors.
    // if (interpreter->AllocateTensors() != kTfLiteOk) {
    //     std::cerr << "Cannot allocate interpreter tensors" << std::endl;
    //     throw;
    // }
    // Set interpreter input.
    const auto* input_tensor = interpreter->input_tensor(0);
    printf("info : %d, %d, %d\n\n", info.height, info.width, info.channel);

    printf("%d", (int)input_tensor->type);
    if (input_tensor->type != kTfLiteUInt8 ||           //
        input_tensor->dims->data[0] != 1 ||             //
        input_tensor->dims->data[1] != info.height ||  //
        input_tensor->dims->data[2] != info.width ||   //
        input_tensor->dims->data[3] != info.channel) {
        std::cerr << "Input tensor shape does not match input image" << std::endl;
        throw;
    }

    printf("success!!!\n\n\n");
}
std::vector<uint8_t> neon_image;


void neon_engine::inference(const std::string image) {
       // Load image.
    this->is_inference = true; 

    // Allocate tensors.
    if (interpreter->AllocateTensors() != kTfLiteOk) {
        std::cerr << "Cannot allocate interpreter tensors" << std::endl;
        throw;
    }

    // Load image.
    int image_bpp, image_width, image_height;
    if (neon_image.size() == 0){ 
       neon_image = tpu::ReadBmpImage("./00374.bmp", &image_width, &image_height, &image_bpp);
    }
    // stbi_load(image_file.c_str(), &image_width, &image_height, &image_bpp, 0);
        
    if (neon_image.empty()) {
        std::cerr << "Cannot read image from " << image << std::endl;
        throw;
    }

    std::copy(neon_image.begin(), neon_image.end(),
                this->interpreter->typed_input_tensor<uint8_t>(0));

    // Run inference.
    if (this->interpreter->Invoke() != kTfLiteOk) {
        std::cerr << "Cannot invoke this->interpreter" << std::endl;
        throw;
    }

    // Get this->interpreter output.
    // auto results = tpu::Sort(tpu::Dequantize(*this->interpreter->output_tensor(0)), threshold);
    // for (auto& result : results)
    //     std::cout << std::setw(7) << std::fixed << std::setprecision(5)
    //             << result.second << tpu::GetLabel(labels, result.first) << std::endl;
    this->is_inference = false; 

    return;
    
}
void neon_engine::deinit() { 
}
