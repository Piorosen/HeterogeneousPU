

import tensorflow as tf
import numpy as np
# import h5py

if __name__ == '__main__':
    parser = argparse.ArgumentParser('tflite to rknn')
    parser.add_argument('-m', dest='modelFile', type=str, required=True, help='텐서플로우 파일 이름')
    parser.add_argument('-o', dest='outputDir', type=str, required=True, help='numpy 출력할 폴더 명')
    args = parser.parse_args()

    model = tf.keras.models.load_model(args.modelFile)
    model.summary()
    # ai_model = tf.keras.applications.vgg16.VGG16(
    #     include_top=True,
    #     weights='imagenet',
    #     input_tensor=None,
    #     input_shape=None,
    #     pooling=None,
    #     classes=1000,
    #     classifier_activation='softmax'
    # )

    for key in model.get_weight_paths().keys():
        data = model.get_weight_paths()[key].shape
        # print(data)
        np.save("./" + args.outputDir + "/" + key + ".npy", np.array(data))
    
# b = h5py.File("./vgg16.h5")
