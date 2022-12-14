# import onnx

# from onnx_tf.backend import prepare

# onnx_model = onnx.load("input_path")  # load onnx model
# tf_rep = prepare(onnx_model)  # prepare tf representation
# tf_rep.export_graph("output_path")  # export the model


import argparse
from onnx2keras import onnx_to_keras
import onnx

if __name__ == '__main__':
    parser = argparse.ArgumentParser('tflite to rknn')
    parser.add_argument('-m', dest='modelFile', type=str, required=True, help='텐서플로우 라이트 파일 이름')
    parser.add_argument('-o', dest='outputFile', type=str, required=True, help='rknn 출력할 파일 명')
    args = parser.parse_args()
    # Load ONNX model
    # onnx_model = onnx.load(args.modelFile)
    
    # Call the converter (input - is the main model input name, can be different for your model)
    k_model = onnx_to_keras(onnx_model, ['input'])
    k_model.save(args.outputFile)
