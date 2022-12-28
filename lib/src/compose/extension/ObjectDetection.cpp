//
// Copyright © 2020 Arm Ltd and Contributors. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <compose/extension/ObjectDetection.h>
// #include "ImageUtils.hpp"

namespace od
{

ObjDetectionPipeline::ObjDetectionPipeline(std::unique_ptr<common::ArmnnNetworkExecutor<float>> executor) :
    m_executor(std::move(executor)){}

void ObjDetectionPipeline::Inference(unsigned char* processed, int width_height_chan, int size_type, common::InferenceResults<float>& result)
{
    m_executor->Run(processed, width_height_chan * size_type, result);
}

void ObjDetectionPipeline::PostProcessing(const std::function<void (int)>& callback)
{
    // DetectedObjects detections = m_decoder->Decode(inferenceResult, m_inputImageSize,
    //                                        m_executor->GetImageAspectRatio(), {});
    if (callback)
    {
        callback(0);
    }
}

// void ObjDetectionPipeline::PreProcessing(const cv::Mat& frame, cv::Mat& processed)
// {
//     m_inputImageSize.m_Height = frame.rows;
//     m_inputImageSize.m_Width = frame.cols;
//     ResizeWithPad(frame, processed, m_processedFrame, m_executor->GetImageAspectRatio());
// }

IPipelinePtr CreatePipeline(common::PipelineOptions& config)
{
    auto executor = std::make_unique<common::ArmnnNetworkExecutor<float>>(config.m_ModelFilePath,
                                                                          config.m_backends,
                                                                          config.m_ProfilingEnabled);
    if (config.m_ModelName == "CHACHA")
    {
        float detectionThreshold = 0.5;
        return std::make_unique<od::ObjDetectionPipeline>(std::move(executor));
    }
    else
    {
        throw std::invalid_argument("Unknown Model name: " + config.m_ModelName + " supplied by user.");
    }
}
}// namespace od
