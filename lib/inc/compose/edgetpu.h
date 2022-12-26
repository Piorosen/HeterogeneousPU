/* Copyright 2021 iwatake2222

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef INFERENCE_HELPER_TENSORFLOW_LITE_
#define INFERENCE_HELPER_TENSORFLOW_LITE_

/* for general */
#include <cstdint>
#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <memory>

/* for Tensorflow Lite */
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/kernels/register.h>

/* for My modules */

class InferenceHelper {
public:
    enum {
        kRetOk = 0,
        kRetErr = -1,
    };

    typedef enum {
        kOpencv,
        kOpencvGpu,
        kTensorflowLite,
        kTensorflowLiteXnnpack,
        kTensorflowLiteGpu,
        kTensorflowLiteEdgetpu,
        kTensorflowLiteNnapi,
        kTensorrt,
        kNcnn,
        kNcnnVulkan,
        kMnn,
        kSnpe,
        kArmnn,
        kNnabla,
        kNnablaCuda,
        kOnnxRuntime,
        kOnnxRuntimeCuda,
        kLibtorch,
        kLibtorchCuda,
        kTensorflow,
        kTensorflowGpu,
        kSample,
    } HelperType;

public:
    static InferenceHelper* Create(const HelperType helper_type);
    static void PreProcessByOpenCV(const InputTensorInfo& input_tensor_info, bool is_nchw, cv::Mat& img_blob);   // use this if the selected inference engine doesn't support pre-process

public:
    virtual ~InferenceHelper() {}
    virtual int32_t SetNumThreads(const int32_t num_threads) = 0;
    virtual int32_t SetCustomOps(const std::vector<std::pair<const char*, const void*>>& custom_ops) = 0;
    virtual int32_t Initialize(const std::string& model_filename, std::vector<InputTensorInfo>& input_tensor_info_list, std::vector<OutputTensorInfo>& output_tensor_info_list) = 0;
    virtual int32_t Finalize(void) = 0;
    virtual int32_t PreProcess(const std::vector<InputTensorInfo>& input_tensor_info_list) = 0;
    virtual int32_t Process(std::vector<OutputTensorInfo>& output_tensor_info_list) = 0;

protected:
    void ConvertNormalizeParameters(InputTensorInfo& tensor_info);

    void PreProcessImage(int32_t num_thread, const InputTensorInfo& input_tensor_info, float* dst);
    void PreProcessImage(int32_t num_thread, const InputTensorInfo& input_tensor_info, uint8_t* dst);
    void PreProcessImage(int32_t num_thread, const InputTensorInfo& input_tensor_info, int8_t* dst);

    template<typename T>
    void PreProcessBlob(int32_t num_thread, const InputTensorInfo& input_tensor_info, T *dst);

protected:
    HelperType helper_type_;
};

class InferenceHelperTensorflowLite : public InferenceHelper {
public:
    InferenceHelperTensorflowLite();
    ~InferenceHelperTensorflowLite() override;
    int32_t SetNumThreads(const int32_t num_threads) override;
    int32_t SetCustomOps(const std::vector<std::pair<const char*, const void*>>& custom_ops) override;
    int32_t Initialize(const std::string& model_filename, std::vector<InputTensorInfo>& input_tensor_info_list, std::vector<OutputTensorInfo>& output_tensor_info_list) override;
    int32_t Finalize(void) override;
    int32_t PreProcess(const std::vector<InputTensorInfo>& input_tensor_info_list) override;
    int32_t Process(std::vector<OutputTensorInfo>& output_tensor_info_list) override;

private:
    int32_t GetInputTensorInfo(InputTensorInfo& tensor_info);
    int32_t GetOutputTensorInfo(OutputTensorInfo& tensor_info);
    void DisplayModelInfo(const tflite::Interpreter& interpreter);

    int32_t SetBufferToTensor(int32_t index, void *data);


private:
    std::vector<char> model_buffer_;
    std::unique_ptr<tflite::FlatBufferModel> model_;
    std::unique_ptr<tflite::ops::builtin::BuiltinOpResolver> resolver_;
    std::unique_ptr<tflite::Interpreter> interpreter_;
    TfLiteDelegate* delegate_;

    int32_t num_threads_;
};

#endif
