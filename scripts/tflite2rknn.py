import numpy as np
import argparse
from rknn.api import RKNN
# import tensorflow as tf

if __name__ == '__main__':
    parser = argparse.ArgumentParser('tflite to rknn')
    parser.add_argument('-m', dest='modelFile', type=str, required=True, help='텐서플로우 라이트 파일 이름')
    parser.add_argument('-o', dest='outputFile', type=str, required=True, help='rknn 출력할 파일 명')
    args = parser.parse_args()
    
    rknn = RKNN(verbose=True)

    # Pre-process config
    print('--> Config model')
    rknn.config(mean_values=[[128, 128, 128]], std_values=[[128, 128, 128]])
    print('done')
    
    # # Load model
    print('--> Loading model')
    ret = rknn.load_tflite(model=args.modelFile)
    if ret != 0:
        print('Load model failed!')
        exit(ret)
    print('done')

    # Build model
    print('--> Building model')
    ret = rknn.build(do_quantization=True, dataset='./dataset.txt')
    if ret != 0:
        print('Build model failed!')
        exit(ret)
    print('done')

    # Export rknn model
    print('--> Export rknn model')
    ret = rknn.export_rknn(args.outputFile)
    if ret != 0:
        print('Export rknn model failed!')
        exit(ret)
    print('done')
    
