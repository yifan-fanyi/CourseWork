#2019.02.06
from __future__ import print_function

import h5py as h5
import numpy as np
import keras
from keras.models import Sequential
from keras.layers import Dense
from keras.optimizers import SGD
from sklearn.cross_validation import train_test_split
from keras.models import load_model

import matplotlib.pyplot as plt

batch_size = 16
num_classes = 2
epochs = 50

data = h5.File('binary_random_sp2019.hdf5', 'r+')
Xh = data['human'][:]
Xm = data['machine'][:]
data.close()

Yh = np.concatenate((np.ones((Xh.shape[0],1)),np.zeros((Xh.shape[0],1))),axis=1)
Ym = np.concatenate((np.zeros((Xm.shape[0],1)),np.ones((Xm.shape[0],1))),axis=1)
X = np.concatenate((Xh, Xm), axis=0)
Y = np.concatenate((Yh, Ym), axis=0)
x_train, x_test, y_train, y_test = train_test_split(X, Y, test_size = 0.2)

model = Sequential()
model.add(Dense(20, activation='relu', input_shape=(20,)))
model.add(Dense(num_classes, activation='softmax'))

model.summary()

model.compile(loss='binary_crossentropy',
              #optimizer=SGD(lr=0.005, decay=0, momentum=0.9, nesterov=True),
              optimizer=keras.optimizers.SGD(lr=0.01, momentum=0.0, decay=0.0, nesterov=False),
              #optimizer=keras.optimizers.RMSprop(lr=0.001, rho=0.9, epsilon=None, decay=0.0),
              #optimizer=keras.optimizers.Adagrad(lr=0.01, epsilon=None, decay=0.0),
              #optimizer=keras.optimizers.Adam(lr=0.001, beta_1=0.9, beta_2=0.999, epsilon=None, decay=0.0, amsgrad=False),
              metrics=['accuracy'])

history = model.fit(x_train, y_train,
                    batch_size=batch_size,
                    epochs=epochs,
                    verbose=1,
                    callbacks=[keras.callbacks.TensorBoard(log_dir='log/', write_images=1, histogram_freq=1)],
                    validation_data=(x_test, y_test))

score = model.evaluate(x_test, y_test, verbose=0)

print('Test loss:', score[0])
print('Test accuracy:', score[1])
model.save('my_model.h5py')

plt.plot(history.history['acc'])
plt.plot(history.history['val_acc'])
plt.title('Model accuracy')
plt.ylabel('Accuracy')
plt.xlabel('Epoch')
plt.legend(['Train', 'Test'], loc='upper left')
plt.show()

# Plot training & validation loss values
plt.plot(history.history['loss'])
plt.plot(history.history['val_loss'])
plt.title('Model loss')
plt.ylabel('Loss')
plt.xlabel('Epoch')
plt.legend(['Train', 'Test'], loc='upper left')
plt.show()
