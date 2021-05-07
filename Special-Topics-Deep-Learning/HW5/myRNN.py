# Alex
# 2019.03.31
# myRNN.py
# LSTM for language recongization

from __future__ import print_function
import tensorflow as tf
import keras
from keras import backend as K
from keras.models import Model
from keras.layers import LSTM, Dense, Dropout, GRU, Input, Flatten
import h5py
from time import time
from keras import callbacks
import os
import numpy as np

num_classes = 3
GRU_nodes = 4

with h5py.File('train.hdf5', 'r') as hf:
    x_train = hf['x_train'][:]
    y_train = hf['y_train'][:]
with h5py.File('val.hdf5', 'r') as hf:
    x_test = hf['x_train'][:]
    y_test = hf['y_train'][:]

x_train = np.moveaxis(x_train,1,2)
x_test = np.moveaxis(x_test,1,2)
y_test = keras.utils.to_categorical(y_test, num_classes)
y_train = keras.utils.to_categorical(y_train, num_classes)

#  model
input = Input(name='input',
              shape=(None,64))
x = GRU(GRU_nodes,
        return_sequences=False,
        name='GRU1')(input)
#x = Flatten()(x)
out = Dense(units=num_classes,
            activation='softmax',
            name='pred')(x)

model = Model(inputs=input, outputs=out)
model.compile(loss=keras.losses.categorical_crossentropy,
              optimizer='adam',
              metrics=['accuracy'])

filepath = "weight/weights-improvement-{epoch:02d}-{val_acc:.2f}.hdf5"
cbs = [callbacks.ModelCheckpoint(filepath,
                                 verbose=1,
                                 save_best_only=False,
                                 mode='max')]

history = model.fit(x_train, y_train,
                    #batch_size=128,
                 epochs=1,
                 class_weight=[0.2,0.5,0.3],
                 validation_data=(x_test, y_test),
                 callbacks=cbs)
model.save('H5Model.h5')

#  transfer to streaming model
streaming_input = Input(name='streaming_input',
                        batch_shape=(1,1,64))
pred_gru = GRU(GRU_nodes,
               name='pred_gru',
               stateful = True)(streaming_input)
rnn_output = Dense(num_classes,
                   name='rnn_output')(pred_gru)

streaming_model = Model(inputs=streaming_input, outputs=rnn_output)
streaming_model.compile(loss='mean_squared_error',
                        optimizer='adam',
                        metrics=['acc'])

old_weights = model.get_weights()
streaming_model.set_weights(old_weights)
streaming_model.save('H5Model_stream.h5')
