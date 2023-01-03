#include <iostream>
#include <compose/mali.h>
#include <stb/stb_image.h>

#include <compose/extension/ObjectDetection.h>

using namespace std;

std::string mali_engine::get_name() const { 
    return "mali";
}

void mali_engine::init(const std::string file, compose::model_info info) { 
    common::PipelineOptions option;
    option.m_ModelFilePath = ("./" + file + "/mali/saved_model.tflite");
    option.m_ModelName  = "CHACHA";
    option.m_ProfilingEnabled = false;
       
    option.m_backends = {"GpuAcc"};
    // option.m_backends = {"CpuAcc", "CpuRef"};
    // CpuAcc, CpuRef, GpuAcc
       printf("MALI!!\n\n\n");
    this->target = od::CreatePipeline(option);
}

       

stbi_uc* data_mali = nullptr;

void mali_engine::inference(const std::string image) {
    this->is_inference = true;
    common::InferenceResults<float> results;
    int width = 224, height = 224, c = 3;
    
    // stbi_uc* data = stbi_load(image.c_str(), &width, &height, &c, 0);
    if (data_mali == nullptr) { 
           data_mali = (stbi_uc*)malloc(224 * 224 * 3 * 4);
    }
    // cout << width << " " << height << " " << c << "\n";
    this->target->Inference((unsigned char*)data_mali, width * height * c, sizeof(float), results);
    // free(data);
    // stbi_image_free(data);
    this->is_inference = false;
    
}
void mali_engine::deinit() { 
    free(data_mali);
}
