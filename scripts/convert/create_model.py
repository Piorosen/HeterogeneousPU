import numpy as np
import tensorflow as tf

if __name__ == '__main__':
    model = tf.keras.applications.vgg16.VGG16(
        include_top=True,
        weights='imagenet',
        input_tensor=None,
        input_shape=None,
        pooling=None,
        classes=1000,
    )
    model.summary()
    model.save("vgg16.h5")
    
    print('done')
