# image to hdf5

# EE599H4
# 2019.02.27
# Yifan Wang

from __future__ import print_function

import numpy as np
import warnings
import keras
from keras.models import Model
from keras.layers import Flatten, Reshape, Dense, Conv2D, Input, MaxPooling2D
from keras.layers.core import Dropout
from keras.preprocessing import image
from keras.utils import layer_utils
from keras.utils.data_utils import get_file
from keras import backend as K
from keras.applications.imagenet_utils import decode_predictions
from keras.applications.imagenet_utils import preprocess_input
from keras_applications.imagenet_utils import _obtain_input_shape
from keras.engine.topology import get_source_inputs
from keras.layers.embeddings import Embedding
from keras import metrics
from sklearn.cross_validation import train_test_split
from keras.utils import plot_model
import csv
import h5py
import os
from keras.models import Sequential
from keras.preprocessing.image import ImageDataGenerator
from time import time
from keras import callbacks
from sklearn.metrics import confusion_matrix, f1_score, precision_score, recall_score

def f1(y_true, y_pred):
    def recall(y_true, y_pred):
        true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
        possible_positives = K.sum(K.round(K.clip(y_true, 0, 1)))
        recall = true_positives / (possible_positives + K.epsilon())
        return recall
    
    def precision(y_true, y_pred):
        true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
        predicted_positives = K.sum(K.round(K.clip(y_pred, 0, 1)))
        precision = true_positives / (predicted_positives + K.epsilon())
        return precision
    precision = precision(y_true, y_pred)
    recall = recall(y_true, y_pred)
    return 2*((precision*recall)/(precision+recall+K.epsilon()))
height = 128
act = 'relu'
classes = 8
model = Sequential()
model.add(Conv2D(64,
                 (3, 3),
                 activation=act,
                 padding='same',
                 name='conv1',
                 input_shape=(height, height, 1)))
model.add(MaxPooling2D((2, 2), strides=(2, 2), name='pool1'))

model.add(Conv2D(128,
                 (3, 3),
                 activation=act,
                 padding='same',
                 name='conv2'))
model.add(MaxPooling2D((2, 2),
                       strides=(2, 2),
                       name='pool2'))
model.add(Conv2D(128,
                 (3,3),
                 activation=act,
                 padding='same',
                 name='conv3'))
model.add(MaxPooling2D((2, 2),
                       strides=(2, 2),
                       name='pool3'))
model.add(Flatten(name='flatten'))
model.add(Dense(256,
                activation=act,
                name='fc1'))
model.add(Dropout(0.5))
model.add(Dense(128,
                activation=act,
                name='fc2'))
model.add(Dropout(0.5))
model.add(Dense(classes,
                activation='softmax',
                name='pred'))

model.compile(loss=keras.losses.categorical_crossentropy,
              optimizer=keras.optimizers.Adadelta(),
              metrics=['accuracy'])

train_datagen = ImageDataGenerator(rescale=1./255,)
train_generator = train_datagen.flow_from_directory('./data/train',
                                                    target_size=(height, height),
                                                    color_mode='grayscale',
                                                    batch_size=128,
                                                    shuffle = True,
                                                    class_mode='categorical')

validation_datagen = ImageDataGenerator(rescale=1./255)
validation_generator = validation_datagen.flow_from_directory('./data/val',
                                                              target_size=(height,height ),
                                                              color_mode='grayscale',
                                                              batch_size=32,
                                                              class_mode='categorical')

filepath = "weights-improvement-{epoch:02d}-{val_acc:.2f}.hdf5"
checkpoint = callbacks.ModelCheckpoint(filepath,
                                       monitor=f1,
                                       verbose=1,
                                       save_best_only=False,
                                       mode='max')

cbs = [callbacks.TensorBoard(log_dir=os.path.join('.', 'logdir',str(int(time()))),
                             histogram_freq=0,
                             write_graph=True,
                             write_images=True),
       checkpoint]

class_weight = np.array([0.0148,0.64,0.0264,0.2732,0.0018,0.0018,0.0204,0.0216])

hist = model.fit_generator(train_generator,
                           steps_per_epoch=100,
                           epochs=30,
                           validation_data=validation_generator,
                           validation_steps=30,
                           class_weight=class_weight,
                           callbacks=cbs)

model.save('H4.h5')
with open('log_sgd_big_32.txt','w') as f:
    f.write(str(hist.history))

