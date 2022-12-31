import numpy as np
import tensorflow as tf
import argparse


def resnet50():
    return tf.keras.applications.resnet50.ResNet50(
        include_top=True,
        weights='imagenet',
        input_tensor=None,
        input_shape=None,
        pooling=None,
        classes=1000,
    )
def resnet101():
    return tf.keras.applications.resnet.ResNet101(
    include_top=True,
    weights='imagenet',
    input_tensor=None,
    input_shape=(224, 224, 3),
    pooling=None,
    classes=1000,
    )


    
def vgg16():
    return tf.keras.applications.vgg16.VGG16(
        include_top=True,
        weights='imagenet',
        input_tensor=None,
        input_shape=None,
        pooling=None,
        classes=1000,
    )
    
def vgg19():
    return tf.keras.applications.vgg19.VGG19(
        include_top=True,
        weights='imagenet',
        input_tensor=None,
        input_shape=None,
        pooling=None,
        classes=1000,
    )
    
def mobilenetV2():
    return tf.keras.applications.mobilenet_v2.MobileNetV2(
        input_shape=(224, 224, 3),
        alpha=1.0,
        include_top=True,
        weights='imagenet',
        input_tensor=None,
        pooling=None,
        classes=1000,
        classifier_activation='softmax',
    )

def efficientnetV2B0():
    return tf.keras.applications.efficientnet_v2.EfficientNetV2B0(
        include_top=True,
        weights='imagenet',
        input_tensor=None,
        input_shape=(224,224,3),
        pooling=None,
        classes=1000,
        classifier_activation='softmax',
        include_preprocessing=True
    )

def efficientnetB0():
    return tf.keras.applications.efficientnet.EfficientNetB0(
    include_top=True,
    weights='imagenet',
    input_tensor=None,
    input_shape=None,
    pooling=None,
    classes=1000,
    classifier_activation='softmax',
    )

if __name__ == '__main__':
    parser = argparse.ArgumentParser('tflite to rknn')
    parser.add_argument('-m', dest='modelFile', type=str, required=True, help='텐서플로우 파일 이름')
    args = parser.parse_args()
    
    model = resnet101()
    model.summary()
    model.save(args.modelFile + ".h5")
    
    print('done')
