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
    option.m_ModelFilePath = ("./" + file + "/tflite/saved_model.tflite");
    option.m_ModelName  = "CHACHA";
    option.m_ProfilingEnabled = true;
       
    option.m_backends = {"GpuAcc"};
    // option.m_backends = {"CpuAcc", "CpuRef"};
    // CpuAcc, CpuRef, GpuAcc
       printf("MALI!!\n\n\n");
    this->target = od::CreatePipeline(option);
}
void mali_engine::inference(const std::string image) {
    common::InferenceResults<float> results;
    int width, height, c;
       
    stbi_uc* data = stbi_load(image.c_str(), &width, &height, &c, 0);
    // cout << width << " " << height << " " << c << "\n";
    this->target->Inference((unsigned char*)data, width * height * c, 1, results);
    stbi_image_free(data);
}
void mali_engine::deinit() { 
    
}