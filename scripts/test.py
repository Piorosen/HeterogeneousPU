

import tensorflow as tf
import numpy as np
# import h5py
from tensorflow.keras.utils import plot_model
model = tf.keras.models.load_model('esrgan-tf2')
plot_model(model, to_file="chacha.jpg", show_shapes=True)
