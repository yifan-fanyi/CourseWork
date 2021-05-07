from keras.objectives import *
from keras.metrics import binary_crossentropy
import keras.backend as K
import tensorflow as tf
import numpy as np

# derive edge from pascal voc prediction
def edge_sparse_crossentropy(y_true, y_pred):
    log_softmax = tf.nn.log_softmax(y_pred)
    tmp1 = K.argmax(log_softmax,axis=-1)
    tmp1 = K.reshape(tmp1, (-1,K.int_shape(tmp1)[-2],K.int_shape(tmp1)[-1],1))
    tmp1 = K.cast(tmp1,'float32')
    tmp = K.conv2d(tmp1, K.ones((3,3,1,1)), strides=(1, 1), padding='same'
                   ,data_format='channels_last')
    y_p = K.abs(9*tmp1 - tmp)
    #y_p = y_p/(y_p+0.000001)
    y_p = np.array(y_p)
    y_p[y_p != 0] = 1
    y_true = K.one_hot(tf.to_int32(K.flatten(y_true)), K.int_shape(y_pred)[-1]+1)
    
    unpacked = tf.unstack(y_true, axis=-1)
    y_true = 1-unpacked[-1]
    
    cross_entropy = K.sum(y_true * y_p)
    
    cross_entropy_mean = cross_entropy/(K.sum(y_true)+0.00001)
                   
    return cross_entropy_mean
'''
def edge_sparse_crossentropy(y_true, y_pred):
    log_softmax = tf.nn.log_softmax(y_pred)
    tmp1 = K.argmax(log_softmax,axis=-1)
    tmp1 = K.reshape(tmp1, (-1,K.int_shape(tmp1)[-2],K.int_shape(tmp1)[-1],1))
    tmp1 = K.cast(tmp1,'float32')
    tmp = K.conv2d(tmp1, K.ones((3,3,1,1)), strides=(1, 1), padding='same'
                   ,data_format='channels_last')
    y_p = K.abs(9*tmp1 - tmp)
    y_p = y_p/(y_p+0.000001)
    
    y_p = np.multiply(K.flatten(y_p),K.flatten(K.max(log_softmax,axis=-1)))
    
    y_true = K.one_hot(tf.to_int32(K.flatten(y_true)), K.int_shape(y_pred)[-1]+1)
    unpacked = tf.unstack(y_true, axis=-1)
    y_true = unpacked[-1]
    
    #y_true = tf.stack(unpacked[:-1], axis=-1)
    cross_entropy = -K.sum(y_true * y_p)
    cross_entropy_mean = K.mean(cross_entropy)
    
    return cross_entropy_mean
'''
# using edge information only
# need to change num_class = 1
def edge_loss(y_true,y_pred):
    y_pred = K.reshape(y_pred, (-1, K.int_shape(y_pred)[-1]))
    log_sigmoid = tf.log_sigmoid(y_pred)

    y_true = K.one_hot(tf.to_int32(K.flatten(y_true)), K.int_shape(y_pred)[-1]+1)
    unpacked = tf.unstack(y_true, axis=-1)
    y_true = unpacked[-1]
    cross_entropy = -K.sum(y_true * log_sigmoid, axis=1)
    cross_entropy_mean = K.mean(cross_entropy)
    
    return cross_entropy_mean

# Softmax cross-entropy loss function for pascal voc segmentation
# and models which do not perform softmax.
# tensorlow only
# ignore white coutor
def softmax_sparse_crossentropy_ignoring_last_label(y_true, y_pred):
    y_pred = K.reshape(y_pred, (-1, K.int_shape(y_pred)[-1]))
    log_softmax = tf.nn.log_softmax(y_pred)
    
    y_true = K.one_hot(tf.to_int32(K.flatten(y_true)), K.int_shape(y_pred)[-1]+1)
    unpacked = tf.unstack(y_true, axis=-1)
    y_true = tf.stack(unpacked[:-1], axis=-1)
    
    cross_entropy = -K.sum(y_true * log_softmax, axis=1)
    cross_entropy_mean = K.mean(cross_entropy)
    
    return cross_entropy_mean


# Softmax cross-entropy loss function for coco segmentation
# and models which expect but do not apply sigmoid on each entry
# tensorlow only
def binary_crossentropy_with_logits(ground_truth, predictions):
    return K.mean(K.binary_crossentropy(ground_truth,
                                        predictions,
                                        from_logits=True),
                  axis=-1)

def edge_seg_crossentropy(y_true, y_pred):
    return softmax_sparse_crossentropy_ignoring_last_label(y_true, y_pred)+edge_sparse_crossentropy(y_true, y_pred)

