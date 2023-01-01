import numpy as np
import argparse
import tensorflow as tf

# 메모리 성능이 부족하여, PC에서 변환 후 넣어주셍 ><
# tf2rknn 프로그램 작성함. 이때, tflite 변환에 메모리 공간 부족으로 인해 파일 분리를 수행함.

if __name__ == '__main__':
    parser = argparse.ArgumentParser('tensorflow to tflite')
    parser.add_argument('-m', dest='modelFile', type=str, required=True, help='텐서플로우 모델 파일 이름')
    parser.add_argument('-o', dest='outputFile', type=str, required=True, help='TFLite 출력할 파일 명')
    args = parser.parse_args()
    
    model = tf.keras.models.load_model(args.modelFile)
    model.summary()
    
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
    converter.representative_dataset = representative_dataset_gen
    converter.inference_input_type = tf.uint8
    converter.inference_output_type = tf.uint8

    tflite_model = converter.convert()
    # Save the model.
    with open(args.outputFile, 'wb') as f:
        f.write(tflite_model)
    
    print('done')
