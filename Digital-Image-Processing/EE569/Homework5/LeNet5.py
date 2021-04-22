#
#  LeNet5.py
#  EE569
#
#  Created by Alex on 2019/3/18.
#  Copyright 2019 Alex. All rights reserved.
#

#  noraml LeNet-5
from __future__ import print_function
import tensorflow as tf
import keras
from keras import backend as K
from keras.datasets import mnist
from keras.models import Sequential
from keras.layers import Dense, Flatten, Conv2D, MaxPooling2D

batch_size = 128
num_classes = 10
epochs = 12
#optimizer = keras.optimizers.Adadelta()
optimizer = keras.optimizers.SGD(lr=0.01, momentum=0.9, decay=0.0001, nesterov=False)

img_rows, img_cols = 28, 28

(x_train, y_train), (x_test, y_test) = mnist.load_data()

x_train = x_train.reshape(x_train.shape[0], img_rows, img_cols, 1)
x_test = x_test.reshape(x_test.shape[0], img_rows, img_cols, 1)
input_shape = (img_rows, img_cols, 1)

x_train = x_train.astype('float32')
x_test = x_test.astype('float32')
x_train /= 255
x_test /= 255

y_train = keras.utils.to_categorical(y_train, num_classes)
y_test = keras.utils.to_categorical(y_test, num_classes)

model = Sequential()
model.add(Conv2D(6, kernel_size=(5, 5),
                 activation='relu',
                 padding='same',
                 name='conv1',
                 input_shape=input_shape))

model.add(MaxPooling2D((2, 2),
                       strides=(2, 2),
                       name='pool1'))
model.add(Conv2D(16,
                 kernel_size=(5, 5),
                 activation='relu',
                 padding='valid',
                 name='conv2'))
model.add(MaxPooling2D((2, 2),
                       strides=(2, 2),
                       name='pool2'))
model.add(Conv2D(120,
                 kernel_size=(5, 5),
                 activation='relu',
                 padding='valid',
                 name='conv3'))
model.add(Flatten())
model.add(Dense(84,
                activation='relu',
                name='dense1'))
model.add(Dense(num_classes,
                activation='softmax',
                name='dense2'))

model.compile(loss=keras.losses.categorical_crossentropy,
              optimizer=optimizer,
              metrics=['accuracy'])

from keras.utils import plot_model
plot_model(model,
           show_shapes=True,
           to_file='LeNet5_model.png')

history = model.fit(x_train,
                    y_train,
                    batch_size=batch_size,
                    epochs=epochs,
                    verbose=1,
                    validation_data=(x_test, y_test))

score = model.evaluate(x_test, y_test, verbose=0)
print('Test loss:', score[0])
print('Test accuracy:', score[1])
score = model.evaluate(1-x_test, y_test, verbose=0)
print('Test loss:', score[0])
print('Test accuracy:', score[1])

import matplotlib.pyplot as plt
plt.plot(history.history['acc'])
plt.plot(history.history['val_acc'])
plt.title('Model accuracy')
plt.ylabel('Accuracy')
plt.xlabel('Epoch')
plt.legend(['Train', 'Test'], loc='upper left')
plt.show()

plt.plot(history.history['loss'])
plt.plot(history.history['val_loss'])
plt.title('Model loss')
plt.ylabel('Loss')
plt.xlabel('Epoch')
plt.legend(['Train', 'Test'], loc='upper left')
plt.show()

model.save('LeNet5_trained.h5')



