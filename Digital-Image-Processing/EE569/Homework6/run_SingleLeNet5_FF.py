#
#  run_SingleLeNet5_FF.py
#  EE569
#
#  Created by Alex on 2019/4/13.
#  Copyright 2019 Alex. All rights reserved.
#
#

from __future__ import print_function
import numpy as np
import keras
from keras import backend as K
from keras.datasets import mnist
from Getkernel_compact import GetKernel
from Getfeature_compact import GetFeature
from Getweight_compact import GetWeight
from LeNet5_FF import *
import data
from tensorflow.python.platform import flags


flags.DEFINE_string("useclasses", "0-9", "Supported format: 0,1,5-9")
F = flags.FLAGS

#  if no such file , you should run the following code
#  get kernel, feature, weight
train_images, train_labels, test_images, test_labels, class_list = data.import_data(F.useclasses)

GetKernel(train_images, train_labels, class_list,
              './weight/pca_params_compact.pkl')

GetFeature('./weight/pca_params_compact.pkl',
           './weight/feat_compact.pkl')

GetWeight('./weight/feat_compact.pkl',
          './weight/llsr_weights_compact_v2.pkl',
          './weight/llsr_bias_compact_v2.pkl',
          train_labels)
    
    
#  test 
(x_train, y_train), (x_test, y_test) = mnist.load_data()
x_test = x_test.reshape(x_test.shape[0], 28, 28, 1)
x_test = x_test.astype('float32')
x_test /= 255
y_test = keras.utils.to_categorical(y_test, 10)

x_train = x_train.reshape(x_train.shape[0], 28, 28, 1)
x_train = x_train.astype('float32')
x_train /= 255
y_train = keras.utils.to_categorical(y_train, 10)

LeNet5_FF('./weight/pca_params_compact.pkl',
          './weight/llsr_weights_compact_v2.pkl',
          './weight/llsr_bias_compact_v2.pkl',
          x_train, y_train, x_test, y_test)
