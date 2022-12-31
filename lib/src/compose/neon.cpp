#include <iostream>
#include <compose/neon.h>
#include <stb/stb_image.h>

#include <compose/extension/ObjectDetection.h>

using namespace std;

std::string neon_engine::get_name() const { 
    return "neon";
}

void neon_engine::init(const std::string file, compose::model_info info) {
       common::PipelineOptions option;
       option.m_ModelFilePath = ("./" + file + "/tflite/saved_model.tflite");
       option.m_ModelName  = "CHACHA";
       option.m_ProfilingEnabled = true;
       // option.m_backends = {"GpuAcc"};
       option.m_backends = {"CpuAcc", "CpuRef"};
       // CpuAcc, CpuRef, GpuAcc
       std::cout <<"1\n";
       this->target = od::CreatePipeline(option);
       std::cout <<"2\n";
}

void neon_engine::inference(const std::string image) {
       this->is_inference = true;

       common::InferenceResults<float> results;
       int width = 224, height = 224, c = 3;
       
       // stbi_uc* data = stbi_load(image.c_str(), &width, &height, &c, 0);
       stbi_uc* data = (stbi_uc*)malloc(224 * 224 * 3 * 4);
       // cout << width << " " << height << " " << c << "\n";
       this->target->Inference((unsigned char*)data, width * height * c, sizeof(float), results);
       free(data);
       // stbi_image_free(data);
       this->is_inference = false;
    
}
void neon_engine::deinit() { 
    
}
