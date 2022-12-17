/*
 * Copyright (c) 2017-2021 Arm Limited.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __ARM_COMPUTE_UTILS_GRAPH_UTILS_H__
#define __ARM_COMPUTE_UTILS_GRAPH_UTILS_H__

#include "arm_compute/core/PixelValue.h"
#include "arm_compute/core/Utils.h"
#include "arm_compute/core/utils/misc/Utility.h"
#include "arm_compute/graph/Graph.h"
#include "arm_compute/graph/ITensorAccessor.h"
#include "arm_compute/graph/Types.h"
#include "arm_compute/runtime/Tensor.h"

#include <array>
#include <random>
#include <string>
#include <vector>

namespace arm_compute
{
namespace graph_utils
{
/** Preprocessor interface **/
class IPreprocessor
{
public:
    /** Default destructor. */
    virtual ~IPreprocessor() = default;
    /** Preprocess the given tensor.
     *
     * @param[in] tensor Tensor to preprocess.
     */
    virtual void preprocess(ITensor &tensor) = 0;
};

/** Caffe preproccessor */
class CaffePreproccessor : public IPreprocessor
{
public:
    /** Default Constructor
     *
     * @param[in] mean  Mean array in RGB ordering
     * @param[in] bgr   Boolean specifying if the preprocessing should assume BGR format
     * @param[in] scale Scale value
     */
    CaffePreproccessor(std::array<float, 3> mean = std::array<float, 3> { { 0, 0, 0 } }, bool bgr = true, float scale = 1.f);
    void preprocess(ITensor &tensor) override;

private:
    template <typename T>
    void preprocess_typed(ITensor &tensor);

    std::array<float, 3> _mean;
    bool  _bgr;
    float _scale;
};

/** TF preproccessor */
class TFPreproccessor : public IPreprocessor
{
public:
    /** Constructor
     *
     * @param[in] min_range Min normalization range. (Defaults to -1.f)
     * @param[in] max_range Max normalization range. (Defaults to 1.f)
     */
    TFPreproccessor(float min_range = -1.f, float max_range = 1.f);

    // Inherited overriden methods
    void preprocess(ITensor &tensor) override;

private:
    template <typename T>
    void preprocess_typed(ITensor &tensor);

    float _min_range;
    float _max_range;
};


/** NumPy accessor class */
class NumPyAccessor final : public graph::ITensorAccessor
{
public:
    /** Constructor
     *
     * @param[in]  npy_path      Path to npy file.
     * @param[in]  shape         Shape of the numpy tensor data.
     * @param[in]  data_type     DataType of the numpy tensor data.
     * @param[in]  data_layout   (Optional) DataLayout of the numpy tensor data.
     * @param[out] output_stream (Optional) Output stream
     */
    NumPyAccessor(std::string npy_path, TensorShape shape, DataType data_type, DataLayout data_layout = DataLayout::NCHW, std::ostream &output_stream = std::cout);
    /** Allow instances of this class to be move constructed */
    NumPyAccessor(NumPyAccessor &&) = default;
    /** Prevent instances of this class from being copied (As this class contains pointers) */
    NumPyAccessor(const NumPyAccessor &) = delete;
    /** Prevent instances of this class from being copied (As this class contains pointers) */
    NumPyAccessor &operator=(const NumPyAccessor &) = delete;

    // Inherited methods overriden:
    bool access_tensor(ITensor &tensor) override;

private:
    template <typename T>
    void access_numpy_tensor(ITensor &tensor, T tolerance);

    Tensor            _npy_tensor;
    const std::string _filename;
    std::ostream     &_output_stream;
};

/** SaveNumPy accessor class */
class SaveNumPyAccessor final : public graph::ITensorAccessor
{
public:
    /** Constructor
     *
     * @param[in] npy_name   Npy file name.
     * @param[in] is_fortran (Optional) If true, save tensor in fortran order.
     */
    SaveNumPyAccessor(const std::string npy_name, const bool is_fortran = false);
    /** Allow instances of this class to be move constructed */
    SaveNumPyAccessor(SaveNumPyAccessor &&) = default;
    /** Prevent instances of this class from being copied (As this class contains pointers) */
    SaveNumPyAccessor(const SaveNumPyAccessor &) = delete;
    /** Prevent instances of this class from being copied (As this class contains pointers) */
    SaveNumPyAccessor &operator=(const SaveNumPyAccessor &) = delete;

    // Inherited methods overriden:
    bool access_tensor(ITensor &tensor) override;

private:
    const std::string _npy_name;
    const bool        _is_fortran;
};

/** Print accessor class
 *  @note The print accessor will print only when asserts are enabled.
 *  */
class PrintAccessor final : public graph::ITensorAccessor
{
public:
    /** Constructor
     *
     * @param[out] output_stream (Optional) Output stream
     * @param[in]  io_fmt        (Optional) Format information
     */
    PrintAccessor(std::ostream &output_stream = std::cout, IOFormatInfo io_fmt = IOFormatInfo());
    /** Allow instances of this class to be move constructed */
    PrintAccessor(PrintAccessor &&) = default;
    /** Prevent instances of this class from being copied (As this class contains pointers) */
    PrintAccessor(const PrintAccessor &) = delete;
    /** Prevent instances of this class from being copied (As this class contains pointers) */
    PrintAccessor &operator=(const PrintAccessor &) = delete;

    // Inherited methods overriden:
    bool access_tensor(ITensor &tensor) override;

private:
    std::ostream &_output_stream;
    IOFormatInfo  _io_fmt;
};

/** Image accessor class */
class ImageAccessor final : public graph::ITensorAccessor
{
public:
    /** Constructor
     *
     * @param[in] filename     Image file
     * @param[in] bgr          (Optional) Fill the first plane with blue channel (default = false - RGB format)
     * @param[in] preprocessor (Optional) Image pre-processing object
     */
    ImageAccessor(std::string filename, bool bgr = true, std::unique_ptr<IPreprocessor> preprocessor = nullptr);
    /** Allow instances of this class to be move constructed */
    ImageAccessor(ImageAccessor &&) = default;

    // Inherited methods overriden:
    bool access_tensor(ITensor &tensor) override;

private:
    bool                           _already_loaded;
    const std::string              _filename;
    const bool                     _bgr;
    std::unique_ptr<IPreprocessor> _preprocessor;
};

/** Detection output accessor class */
class DetectionOutputAccessor final : public graph::ITensorAccessor
{
public:
    /** Constructor
     *
     * @param[in]  labels_path        Path to labels text file.
     * @param[in]  imgs_tensor_shapes Network input images tensor shapes.
     * @param[out] output_stream      (Optional) Output stream
     */
    DetectionOutputAccessor(const std::string &labels_path, std::vector<TensorShape> &imgs_tensor_shapes, std::ostream &output_stream = std::cout);
    /** Allow instances of this class to be move constructed */
    DetectionOutputAccessor(DetectionOutputAccessor &&) = default;
    /** Prevent instances of this class from being copied (As this class contains pointers) */
    DetectionOutputAccessor(const DetectionOutputAccessor &) = delete;
    /** Prevent instances of this class from being copied (As this class contains pointers) */
    DetectionOutputAccessor &operator=(const DetectionOutputAccessor &) = delete;

    // Inherited methods overriden:
    bool access_tensor(ITensor &tensor) override;

private:
    template <typename T>
    void access_predictions_tensor(ITensor &tensor);

    std::vector<std::string> _labels;
    std::vector<TensorShape> _tensor_shapes;
    std::ostream            &_output_stream;
};

/** Result accessor class */
class TopNPredictionsAccessor final : public graph::ITensorAccessor
{
public:
    /** Constructor
     *
     * @param[in]  labels_path   Path to labels text file.
     * @param[in]  top_n         (Optional) Number of output classes to print
     * @param[out] output_stream (Optional) Output stream
     */
    TopNPredictionsAccessor(const std::string &labels_path, size_t top_n = 5, std::ostream &output_stream = std::cout);
    /** Allow instances of this class to be move constructed */
    TopNPredictionsAccessor(TopNPredictionsAccessor &&) = default;
    /** Prevent instances of this class from being copied (As this class contains pointers) */
    TopNPredictionsAccessor(const TopNPredictionsAccessor &) = delete;
    /** Prevent instances of this class from being copied (As this class contains pointers) */
    TopNPredictionsAccessor &operator=(const TopNPredictionsAccessor &) = delete;

    // Inherited methods overriden:
    bool access_tensor(ITensor &tensor) override;

private:
    template <typename T>
    void access_predictions_tensor(ITensor &tensor);

    std::vector<std::string> _labels;
    std::ostream            &_output_stream;
    size_t                   _top_n;
};

/** Numpy Binary loader class*/
class NumPyBinLoader final : public graph::ITensorAccessor
{
public:
    /** Default Constructor
     *
     * @param[in] filename    Binary file name
     * @param[in] file_layout (Optional) Layout of the numpy tensor data. Defaults to NCHW
     */
    NumPyBinLoader(std::string filename, DataLayout file_layout = DataLayout::NCHW);
    /** Allows instances to move constructed */
    NumPyBinLoader(NumPyBinLoader &&) = default;

    // Inherited methods overriden:
    bool access_tensor(ITensor &tensor) override;

private:
    bool              _already_loaded;
    const std::string _filename;
    const DataLayout  _file_layout;
};

/** Generates appropriate weights accessor according to the specified path
 *
 * @note If path is empty will generate a DummyAccessor else will generate a NumPyBinLoader
 *
 * @param[in] path        Path to the data files
 * @param[in] data_file   Relative path to the data files from path
 * @param[in] file_layout (Optional) Layout of file. Defaults to NCHW
 *
 * @return An appropriate tensor accessor
 */
inline std::unique_ptr<graph::ITensorAccessor> get_weights_accessor(const std::string &path,
                                                                    const std::string &data_file,
                                                                    DataLayout         file_layout = DataLayout::NCHW)
{
    if(path.empty())
    {
        throw 0;
    }
    else
    {
        return std::make_unique<NumPyBinLoader>(path + data_file, file_layout);
    }
}

/** Generates print tensor accessor
 *
 * @param[out] output_stream (Optional) Output stream
 *
 * @return A print tensor accessor
 */
inline std::unique_ptr<graph::ITensorAccessor> get_print_output_accessor(std::ostream &output_stream = std::cout)
{
    return std::make_unique<PrintAccessor>(output_stream);
}

/** Permutes a given tensor shape given the input and output data layout
 *
 * @param[in] tensor_shape    Tensor shape to permute
 * @param[in] in_data_layout  Input tensor shape data layout
 * @param[in] out_data_layout Output tensor shape data layout
 *
 * @return Permuted tensor shape
 */
inline TensorShape permute_shape(TensorShape tensor_shape, DataLayout in_data_layout, DataLayout out_data_layout)
{
    if(in_data_layout != out_data_layout)
    {
        arm_compute::PermutationVector perm_vec = (in_data_layout == DataLayout::NCHW) ? arm_compute::PermutationVector(2U, 0U, 1U) : arm_compute::PermutationVector(1U, 2U, 0U);
        arm_compute::permute(tensor_shape, perm_vec);
    }
    return tensor_shape;
}

/** Utility function to return the TargetHint
 *
 * @param[in] target Integer value which expresses the selected target. Must be 0 for Arm® Neon™ or 1 for OpenCL or 2 (OpenCL with Tuner)
 *
 * @return the TargetHint
 */
inline graph::Target set_target_hint(int target)
{
    ARM_COMPUTE_ERROR_ON_MSG(target > 2, "Invalid target. Target must be 0 (NEON), 1 (OpenCL), 2 (OpenCL + Tuner)");
    if((target == 1 || target == 2))
    {
        return graph::Target::CL;
    }
    else
    {
        return graph::Target::NEON;
    }
}
} // namespace graph_utils
} // namespace arm_compute

#endif /* __ARM_COMPUTE_UTILS_GRAPH_UTILS_H__ */
