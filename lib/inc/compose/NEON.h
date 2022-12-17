#ifndef MAIN_COMPOSE_NEON_ARMCL_H
#define MAIN_COMPOSE_NEON_ARMCL_H

#include <string>
#include <memory>

#include <arm_compute/graph.h>
// #ifdef ARM_COMPUTE_CL
// #include <arm_compute/runtime/CL/Utils.h>
// #endif /* ARM_COMPUTE_CL */
#include <support/ToolchainSupport.h>
#include <utils/GraphUtils.h>

using namespace arm_compute;
using namespace arm_compute::utils;
using namespace arm_compute::graph::frontend;
using namespace arm_compute::graph_utils;

void run_neon() {
    Stream graph(0, "AlexNet");

    std::string data_path = ".";
    // Create a preprocessor object
    const std::array<float, 3> mean_rgb{ { 122.68f, 116.67f, 104.01f } };
    std::unique_ptr<IPreprocessor> preprocessor = std::make_unique<CaffePreproccessor>(mean_rgb);

    // Create input descriptor
    const auto        operation_layout = arm_compute::DataLayout::NHWC;
    const TensorShape tensor_shape     = permute_shape(TensorShape(227U, 227U, 3U, 1), DataLayout::NCHW, operation_layout);
    TensorDescriptor  input_descriptor = TensorDescriptor(tensor_shape, arm_compute::DataType::F32).set_layout(operation_layout);

    // Set weights trained layout
    const DataLayout weights_layout = DataLayout::NCHW;

    graph << arm_compute::graph::Target::NEON
            << arm_compute::graph::ConvolutionMethod::Default
            << arm_compute::graph::FastMathHint::Disabled
            << InputLayer(input_descriptor, std::make_unique<ImageAccessor>("./go_kart.ppm", true, std::move(preprocessor)))
            // Layer 1
            << ConvolutionLayer(
                11U, 11U, 96U,
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv1_w.npy", weights_layout),
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv1_b.npy"),
                PadStrideInfo(4, 4, 0, 0))
            .set_name("conv1")
            << ActivationLayer(ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU)).set_name("relu1")
            << NormalizationLayer(NormalizationLayerInfo(NormType::CROSS_MAP, 5, 0.0001f, 0.75f)).set_name("norm1")
            << PoolingLayer(PoolingLayerInfo(PoolingType::MAX, 3, operation_layout, PadStrideInfo(2, 2, 0, 0))).set_name("pool1")
            // Layer 2
            << ConvolutionLayer(
                5U, 5U, 256U,
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv2_w.npy", weights_layout),
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv2_b.npy"),
                PadStrideInfo(1, 1, 2, 2), 2)
            .set_name("conv2")
            << ActivationLayer(ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU)).set_name("relu2")
            << NormalizationLayer(NormalizationLayerInfo(NormType::CROSS_MAP, 5, 0.0001f, 0.75f)).set_name("norm2")
            << PoolingLayer(PoolingLayerInfo(PoolingType::MAX, 3, operation_layout, PadStrideInfo(2, 2, 0, 0))).set_name("pool2")
            // Layer 3
            << ConvolutionLayer(
                3U, 3U, 384U,
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv3_w.npy", weights_layout),
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv3_b.npy"),
                PadStrideInfo(1, 1, 1, 1))
            .set_name("conv3")
            << ActivationLayer(ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU)).set_name("relu3")
            // Layer 4
            << ConvolutionLayer(
                3U, 3U, 384U,
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv4_w.npy", weights_layout),
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv4_b.npy"),
                PadStrideInfo(1, 1, 1, 1), 2)
            .set_name("conv4")
            << ActivationLayer(ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU)).set_name("relu4")
            // Layer 5
            << ConvolutionLayer(
                3U, 3U, 256U,
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv5_w.npy", weights_layout),
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv5_b.npy"),
                PadStrideInfo(1, 1, 1, 1), 2)
            .set_name("conv5")
            << ActivationLayer(ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU)).set_name("relu5")
            << PoolingLayer(PoolingLayerInfo(PoolingType::MAX, 3, operation_layout, PadStrideInfo(2, 2, 0, 0))).set_name("pool5")
            // Layer 6
            << FullyConnectedLayer(
                4096U,
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/fc6_w.npy", weights_layout),
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/fc6_b.npy"))
            .set_name("fc6")
            << ActivationLayer(ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU)).set_name("relu6")
            // Layer 7
            << FullyConnectedLayer(
                4096U,
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/fc7_w.npy", weights_layout),
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/fc7_b.npy"))
            .set_name("fc7")
            << ActivationLayer(ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU)).set_name("relu7")
            // Layer 8
            << FullyConnectedLayer(
                1000U,
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/fc8_w.npy", weights_layout),
                get_weights_accessor(data_path, "/cnn_data/alexnet_model/fc8_b.npy"))
            .set_name("fc8")
            // Softmax
            << SoftmaxLayer().set_name("prob")
            << OutputLayer(std::make_unique<TopNPredictionsAccessor>("./labels.txt", 5, std::cout));

        // Finalize graph
        GraphConfig config;
        graph.finalize(arm_compute::graph::Target::NEON, config);

        // Run graph
        graph.run();
}

#endif // MAIN_COMPOSE_NEON_ARMCL_H
