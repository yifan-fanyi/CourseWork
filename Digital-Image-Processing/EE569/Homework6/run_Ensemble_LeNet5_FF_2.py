#  Alex
#
#  run_Ensemble_LeNet5_FF_1.py
#  EE569
#
#  Created by Alex on 2019/4/13.
#  Copyright 2019 Alex. All rights reserved.
#
#  ensemble FF-CNN using by add 9 laws filter as feature

from __future__ import print_function
import numpy as np
import keras
from keras import backend as K
import tensorflow as tf
from keras.datasets import mnist
from Getkernel_compact import GetKernel
from Getfeature_compact import GetFeature
from Getweight_compact import GetWeight
from data import *
from LeNet5_FF import *
import os
import cv2
import sklearn
from sklearn.decomposition import PCA
from numpy import linalg as LA
os.environ['KMP_DUPLICATE_LIB_OK']='True'

#  seperate into 10 class
(x_train, y_train), (x_test, y_test) = mnist.load_data()
x_train = x_train.reshape(x_train.shape[0], 28, 28)

# use subset for faster compution

#x_train_sep, y_train_sep = SeparateDate(x_train, y_train)
#x_train = x_train_sep[0:20000]
#y_train = y_train_sep[0:20000]

x_test = x_test.reshape(x_test.shape[0], 28, 28)
x_train = x_train.astype('float32')
x_test = x_test.astype('float32')
x_train /= 255
x_test /= 255
y_train = y_train.astype('int32')

train_images_s = LawsFilter(x_train)
train_labels = keras.utils.to_categorical(y_train, 10)
train_images = np.pad(train_images_s, ((0,0),(0,0),(2,2),(2,2),(0,0)), mode='constant')

test_images =  np.array(LawsFilter(x_test))
test_labels = keras.utils.to_categorical(y_test, 10)

prediction = []
prediction1 = []
class_weight = np.array([0.9704, 0.9499, 0.9284, 0.9015, 0.8651,
                         0.9615, 0.9549, 0.9380, 0.9190, 0.8774])
for i in range(0,10):
    kernel_name = './weight/pca_params_compact_'+str(i)+'.pkl'
    feature_name = './weight/feat_compact_'+str(i)+'.pkl'
    weight_name = './weight/llsr_weights_compact_v2_'+str(i)+'.pkl'
    bias_name = './weight/llsr_bias_compact_v2_'+str(i)+'.pkl'
    
    GetKernel(train_images[i],
            train_labels,
            [0,1,2,3,4,5,6,7,8,9],
            kernel_name)
    GetFeature(kernel_name,
            feature_name,
            train_images[i])
    
    GetWeight(feature_name,
            weight_name,
            bias_name,
            y_train)
    
    print('start %2d th test'%(i+1))
    pred1, pred = LeNet5_FF(kernel_name,
                            weight_name,
                            bias_name,
                            train_images_s[i],
                            train_labels,
                            test_images[i],
                            test_labels)
    prediction.append(class_weight[i]*pred)
    prediction1.append(class_weight[i]*pred1)

prediction1 = np.array(prediction1)
prediction = np.array(prediction)

print(prediction1.shape)
prediction1 = np.moveaxis(prediction1, 0, 2)
prediction1 = prediction1.reshape(-1,100)

prediction = np.moveaxis(prediction, 0, 2)
prediction = prediction.reshape(-1,100)
print(prediction1.shape)


labels = keras.utils.to_categorical(y_train, 10)
weight = np.matmul(LA.pinv(prediction1), labels).astype(np.float32)
prediction1 = np.matmul(prediction1, weight)
pred_labels = np.argmax(prediction1, axis=1)
acc_train = sklearn.metrics.accuracy_score(y_train, pred_labels)
print('training acc is {}'.format(acc_train))



'''
pca = PCA(n_components=10, svd_solver='full')
prediction = pca.fit_transform(prediction, 10)
'''
print(weight.shape)
prediction = np.dot(prediction, weight)

ypred = np.eye(10)[np.argmax(prediction, axis=1)]
acc = np.sum(ypred*test_labels)/ypred.shape[0]
print('ensemble: %5f' %acc)

CompOverlap(ypred, test_labels, x_test.reshape(-1,28,28,1))


