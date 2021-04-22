#
#  LeNet5_FF.py
#  EE569
#
#  modified from https://github.com/davidsonic/Interpretable_CNN/blob/master/adv_attack/mnist_ff_model.py
#  single FF-CNN test

import keras
import pickle
import numpy as np
from keras.datasets import mnist
from keras.layers import Conv2D, MaxPool2D, Dense, Flatten, Dropout, Subtract
from keras.layers.normalization import BatchNormalization
from keras.models import Sequential
from keras import optimizers
from keras.models import load_model
import os
from keras import backend as K

K.tensorflow_backend._get_available_gpus()

class Load_FF_Par():
    def __init__(self, pca_name, weight_name, bias_name):
        self.pca_name = pca_name
        self.weight_name = weight_name
        self.bias_name = bias_name
        self.pca_params = 0
        self.weight = 0
        self.biases = 0

    def open_par(self):
        fr = open(self.pca_name, 'rb')
        self.pca_params = pickle.load(fr)
        fr.close()

        fr = open(self.weight_name, 'rb')
        self.weights = pickle.load(fr)
        fr.close()

        fr = open(self.bias_name, 'rb')
        self.biases = pickle.load(fr)
        fr.close()

    def bias_init_conv_1(self, shape, dtype=None):
        tmp_bias = (-1*np.matmul(self.pca_params['Layer_0/feature_expectation'],
                               np.transpose(self.pca_params['Layer_0/kernel']))).reshape(shape)
        return tmp_bias

    def weight_init_conv_1(self, shape, dtype=None):
        #print 'init conv1 function: {}'.format( pca_params['Layer_0/kernel'])
        weight = self.pca_params['Layer_0/kernel'].astype(np.float32)
        weight = np.moveaxis(weight, 1, 0)
        return weight.reshape(shape)

    def weight_init_conv_2(self, shape, dtype=None):
        weight = self.pca_params['Layer_1/kernel'].astype(np.float32)
        weight = np.moveaxis(weight, 1,0)
        weight = weight.reshape(6,5,5, 16)
        weight = np.moveaxis(weight, 0, 2)
        return weight.reshape(shape)

    def bias_init_conv_2(self, shape, dtype=None):
        weight = np.transpose(self.pca_params['Layer_1/kernel'].astype(np.float32))
        tmp_bias = (-1 * np.matmul(self.pca_params['Layer_1/feature_expectation'].astype(np.float32), weight)).reshape(shape)

        bias = np.zeros(150)
        bias = bias + 1/np.sqrt(150)*self.pca_params['Layer_1/bias'].astype(np.float32)

        bias1 = np.matmul(bias, weight)

        bias2 = np.zeros(shape, dtype=np.float32)
        bias2[0] = -1
        bias2 = bias2 * self.pca_params['Layer_1/bias'].astype(np.float32)

        bias_final = tmp_bias + bias1 + bias2
        return bias_final.reshape(shape)

    def weight_init_fc_1(self, shape, dtype=None):
        return self.weights['0 LLSR weight'].reshape(shape)

    def bias_init_fc_1(self, shape, dtype=None):
        return self.biases['0 LLSR bias'].reshape(shape)

    def weight_init_fc_2(self, shape, dtype=None):
        return self.weights['1 LLSR weight'].reshape(shape)

    def bias_init_fc_2(self, shape, dtype=None):
        return self.biases['1 LLSR bias'].reshape(shape)

    def weight_init_fc_3(self, shape, dtype=None):
        return self.weights['2 LLSR weight'].reshape(shape)

    def bias_init_fc_3(self, shape, dtype=None):
        return self.biases['2 LLSR bias'].reshape(shape)

def LeNet5_FF(pca_name, weight_name, bias_name, x_train, y_train, x_test, y_test):
    par = Load_FF_Par(pca_name, weight_name, bias_name)
    par.open_par()
    
    lenet = Sequential()
    lenet.add(Conv2D(6,
                     activation=None,
                     kernel_size=5,
                     strides=1,
                     padding='same',
                     data_format='channels_last',
                     kernel_initializer=par.weight_init_conv_1,
                     input_shape=(28, 28, 1),
                     bias_initializer=par.bias_init_conv_1) ) # bias?
    lenet.add(MaxPool2D(pool_size=2,
                        strides=2))
    lenet.add(Conv2D(16,
                     activation=None,
                     kernel_size=5,
                     strides=1,
                     padding='valid',
                     data_format='channels_last',
                     kernel_initializer=par.weight_init_conv_2,
                     bias_initializer=par.bias_init_conv_2))
    lenet.add(MaxPool2D(pool_size=2,
                        strides=2))
    lenet.add(Flatten())
    lenet.add(Dense(120,
                    activation='relu',
                    kernel_initializer=par.weight_init_fc_1,
                    bias_initializer=par.bias_init_fc_1))
    lenet.add(Dense(84,
                    activation='relu',
                    kernel_initializer=par.weight_init_fc_2,
                    bias_initializer=par.bias_init_fc_2))
    lenet.add(Dense(10,
                    activation='softmax',
                    kernel_initializer=par.weight_init_fc_3,
                    bias_initializer=par.bias_init_fc_3))
    lenet.compile(loss='categorical_crossentropy',
                  optimizer=keras.optimizers.SGD(lr=0.001),
                  metrics=['accuracy'])

    if y_train.all != None:
        scores = lenet.evaluate(x_train, y_train, verbose=1)
        print('Train loss: %5f'%scores[0])
        print('Train accuracy: %5f'%scores[1])

        scores = lenet.evaluate(x_test, y_test, verbose=1)
        print('Test loss: %5f'%scores[0])
        print('Test accuracy: %5f'%scores[1])
    return lenet.predict(x_train), lenet.predict(x_test)

