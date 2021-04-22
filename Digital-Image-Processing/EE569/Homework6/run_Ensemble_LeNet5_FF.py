#  Alex
#
#  run_Ensemble_LeNet5_FF.py
#  EE569
#
#  Created by Alex on 2019/4/13.
#  Copyright 2019 Alex. All rights reserved.
#
#  ensemble FF-CNN using by seperating data to 10 non-overlapping subsets

from __future__ import print_function
import numpy as np
import keras
from keras import backend as K
from keras.datasets import mnist
from Getkernel_compact import GetKernel
from Getfeature_compact import GetFeature
from Getweight_compact import GetWeight
from LeNet5_FF import *
import os
os.environ['KMP_DUPLICATE_LIB_OK']='True'

#  seperate into 10 class
x_train, y_train, x_test, y_test = MNIST_GP()
y_test = keras.utils.to_categorical(y_test, 10)
y_train = y_train.astype('int32')

x_train_sep, y_train_sep = SeparateDate(x_train, y_train)

train_images = np.pad(x_train_sep, ((0,0),(2,2),(2,2),(0,0)), mode='constant')
train_labels = keras.utils.to_categorical(y_train_sep, 10)

prediction = np.zeros((10000,10))

for i in range(0,10):
    kernel_name = './weight/pca_params_compact_'+str(i)+'.pkl'
    weight_name = './weight/llsr_weights_compact_v2_'+str(i)+'.pkl'
    bias_name = './weight/llsr_bias_compact_v2_'+str(i)+'.pkl'
    feature_name = './weight/feat_compact_'+str(i)+'.pkl'

    GetKernel(train_images[5000*i:5000*(i+1)],
              train_labels[5000*i:5000*(i+1)],
              [0,1,2,3,4,5,6,7,8,9],
              kernel_name)
    GetFeature(kernel_name,
               feature_name,
               train_images[5000*i:5000*(i+1)])
    GetWeight(feature_name,
              weight_name,
              bias_name,
              y_train_sep[5000*i:5000*(i+1)])

    print('start %2d th test'%(i+1))
    pred = LeNet5_FF(kernel_name,
                     weight_name,
                     bias_name,
                     x_train_sep[5000*i:5000*(i+1)],
                     train_labels[5000*i:5000*(i+1)],
                     x_test,
                     y_test)
    prediction +=  np.array(pred)

ypred = np.eye(10)[np.argmax(prediction, axis=1)]
acc = np.sum(ypred*y_test)/ypred.shape[0]
print('ensemble: %5f' %acc)

CompOverlap(ypred, y_test, x_test)
