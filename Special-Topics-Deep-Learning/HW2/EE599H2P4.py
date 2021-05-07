#2019.01.25
import numpy as np
import h5py as h5
from sklearn.neural_network import MLPRegressor
from sklearn.cross_validation import train_test_split
from sklearn.metrics import mean_squared_error

import pickle

f = h5.File('lms_fun_v3.hdf5', 'r')

mismatched_v = f['mismatched_v'][:]
mismatched_y = f['mismatched_y'][:]

f.close

mismatched_v = mismatched_v.reshape(300600, -1)
mismatched_y = mismatched_y.reshape(300600, -1)

train_x, test_x, train_y, test_y = train_test_split(mismatched_v, mismatched_y, test_size = 0.3)

model = MLPRegressor(hidden_layer_sizes=(6,4), activation='relu', solver='adam', alpha=0.0001, max_iter=600)
model.fit(train_x, np.ravel(train_y, order='C'))
pickle.dump(model, open('nn.pkl', 'wb'))

ypred = model.predict(test_x).reshape(-1,1)
mse = mean_squared_error(test_y, ypred)
print('MSE: ',mse)
