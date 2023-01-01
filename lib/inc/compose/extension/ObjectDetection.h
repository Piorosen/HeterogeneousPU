//
// Copyright © 2020 Arm Ltd and Contributors. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <compose/extension/armnn.h>
// # include "ImageUtils.hpp"

namespace od
{
/**
 * Generic object detection pipeline with 3 steps: data pre-processing, inference execution and inference
 * result post-processing.
 *
 */
class ObjDetectionPipeline {
public:

    /**
     * Creates object detection pipeline with given network executor and decoder.
     * @param executor - unique pointer to inference runner
     * @param decoder - unique pointer to inference results decoder
     */
    ObjDetectionPipeline(std::unique_ptr<common::ArmnnNetworkExecutor<float>> executor);

    /**
     * @brief Standard image pre-processing implementation.
     *
     * Re-sizes an image keeping aspect ratio, pads if necessary to fit the network input layer dimensions.

     * @param[in] frame - input image, expected data type is uint8.
     * @param[out] processed - output image, data type is preserved.
     */
    // virtual void PreProcessing(const cv::Mat& frame, cv::Mat& processed);

    /**
     * @brief Executes inference
     *
     * Calls inference runner provided during instance construction.
     *
     * @param[in] processed - input inference data. Data type should be aligned with input tensor.
     * @param[out] result - raw floating point inference results.
     */
    void Inference(unsigned char* processed, int width_height_chan, int size_type, common::InferenceResults<float>& result);

    /**
     * @brief Standard inference results post-processing implementation.
     *
     * Decodes inference results using decoder provided during construction.
     *
     * @param[in] inferenceResult - inference results to be decoded.
     * @param[in] callback - a function to be called after successful inference results decoding.
     */
    virtual void PostProcessing(const std::function<void (int)>& callback);
    // virtual void PostProcessing(common::InferenceResults<float>& inferenceResult,
    //                             const std::function<void (int)>& callback);

protected:
    std::unique_ptr<common::ArmnnNetworkExecutor<float>> m_executor;
    // std::unique_ptr<IDetectionResultDecoder> m_decoder;
    common::Size m_inputImageSize{};
    // cv::Mat m_processedFrame;
};


using IPipelinePtr = std::unique_ptr<od::ObjDetectionPipeline>;

/**
 * Constructs object detection pipeline based on configuration provided.
 *
 * @param[in] config - object detection pipeline configuration.
 *
 * @return unique pointer to object detection pipeline.
 */
IPipelinePtr CreatePipeline(common::PipelineOptions& config);

}// namespace od