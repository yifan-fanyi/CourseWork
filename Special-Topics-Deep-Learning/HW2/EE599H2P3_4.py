#2019.01.25
import numpy as np
import h5py as h5
import matplotlib.pyplot as plt
from sklearn.neural_network import MLPRegressor
from numpy.linalg import inv

f = h5.File('lms_fun_v3.hdf5', 'r')
mismatched_v = f['mismatched_v'][:]
mismatched_x = f['mismatched_x'][:]
mismatched_y = f['mismatched_y'][:]
f.close

u = 0.05
w = np.random.randn(600, 3)
y = np.zeros((600, 501))
e = np.zeros((600, 501))

for j in range(600):
    for i in range(501):
        y[j,i] = np.dot(w[j], mismatched_v[j,i])
        e[j,i] = mismatched_y[j,i] - y[j,i]
        w = w + u*e[j,i]*mismatched_v[j,i]

avgv = np.average(mismatched_v,axis=0)
avgy = np.average(mismatched_y,axis=0)
Rxx = np.dot(avgv.T, avgv)
Rxy = np.dot(avgv.T, avgy)
Ryy = np.average(np.dot(avgy.T, avgy))
llse = Ryy - np.dot(Rxy.T,np.dot(inv(Rxx),Rxy))
llse = 20*np.log10(abs(llse))
print(llse)

fig = plt.figure(figsize = (9,9))
ax1 = plt.subplot()
ax1.set_title('Learning Curve')
ax1.plot(np.arange(501), np.average(20*np.log10(abs(e)), axis=0),label='MSE')
ax1.plot(np.arange(501), llse*np.ones(501),label='LLSE')
ax1.set_ylabel('average error (dB)')
ax1.set_xlabel('updates')
plt.savefig('H2P3_4a.png')

