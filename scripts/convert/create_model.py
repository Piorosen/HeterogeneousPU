import numpy as np
import tensorflow as tf

if __name__ == '__main__':
    model = tf.keras.applications.resnet50.ResNet50(
        include_top=True,
        weights='imagenet',
        input_tensor=None,
        input_shape=None,
        pooling=None,
        classes=1000,
    )
    model.summary()
    model.save("resnet50.h5")
    
    print('done')
