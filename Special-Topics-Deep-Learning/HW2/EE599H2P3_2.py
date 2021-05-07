#2019.01.25
import numpy as np
import h5py as h5
import matplotlib.pyplot as plt
from sklearn.neural_network import MLPRegressor

f = h5.File('lms_fun_v3.hdf5', 'r')
matched_10_v = f['matched_10_v'][:]
matched_10_y = f['matched_10_y'][:]
matched_10_z = f['matched_10_z'][:]

matched_3_v = f['matched_3_v'][:]
matched_3_y = f['matched_3_y'][:]
matched_3_z = f['matched_3_z'][:]
f.close

u = 0.15
w = np.random.randn(600, 3)
y = np.zeros((600, 501))
e = np.zeros((600, 501))

w1 = np.random.randn(600, 3)
y1 = np.zeros((600, 501))
e1 = np.zeros((600, 501))

for j in range(600):
    for i in range(501):
        y[j][i] = np.dot(w[j], matched_3_v[j][i])
        e[j][i] = matched_3_z[j][i] - y[j][i]
        w = w + u*e[j][i]*matched_3_v[j][i]
        
        y1[j][i] = np.dot(w1[j], matched_3_v[j][i])
        e1[j][i] = matched_3_y[j][i] - y1[j][i]
        w1 = w1 + u*e1[j][i]*matched_3_v[j][i]

fig = plt.figure(figsize = (9,9))
ax1 = plt.subplot()
ax1.set_title('Learning Curve')
ax1.plot(np.arange(501), np.average(abs(e)**2, axis=0),label='z')
ax1.plot(np.arange(501), np.average(abs(e1)**2, axis=0),label='y')
ax1.plot(np.arange(501), 0.5011*np.ones(501),label='MSE')
ax1.legend()
ax1.set_ylabel('average error')
ax1.set_xlabel('updates')
plt.savefig('3dB_0d15.png')

