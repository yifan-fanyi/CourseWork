#2019.01.25
import numpy as np
import h5py as h5
import matplotlib.pyplot as plt
from sklearn.neural_network import MLPRegressor

f = h5.File('lms_fun_v3.hdf5', 'r')
time_varying_coefficents = f['timevarying_coefficents'][:]
time_varying_x = f['timevarying_x'][:]
time_varying_y = f['timevarying_y'][:]
time_varying_z = f['timevarying_z'][:]
f.close

u = 0.05
w = np.random.randn(501, 3)
y = np.zeros((501))
e = np.ones((501))
for i in range(2,500):
    v = np.array([time_varying_x[i],time_varying_x[i-1],time_varying_x[i-2]])
    y[i] = np.dot(w[i], v)
    e[i] = time_varying_z[i] - y[i]
    w[i+1] = w[i] + u*e[i]*v

fig = plt.figure(figsize = (9,9))
ax1 = plt.subplot()
ax1.set_title('coefficients')
ax1.plot(np.arange(501), time_varying_coefficents[:,0], label='coef_1')
ax1.plot(np.arange(501), w[:,0], label='coef_1_est')
ax1.plot(np.arange(501), time_varying_coefficents[:,1], label='coef_2')
ax1.plot(np.arange(501), w[:,1], label='coef_2_est')
ax1.plot(np.arange(501), time_varying_coefficents[:,2], label='coef_3')
ax1.plot(np.arange(501), w[:,2], label='coef_3_est')
ax1.set_ylabel('coefficents, learning_rate=0.05')
ax1.set_xlabel('n')
ax1.legend()
plt.savefig('H2P3_3a.png')

fig = plt.figure(figsize = (9,9))
ax2 = plt.subplot()
ax2.plot(np.arange(50), y[450:500],label='predict y')
ax2.plot(np.arange(50), time_varying_z[450:500],label='original z')
ax2.legend()
plt.savefig('H2P3_3b.png')

fig = plt.figure(figsize = (9,9))
ax3 = plt.subplot()
ax3.set_title('Learning Curve')
ax3.plot(np.arange(501), 20*np.log10(abs(e)))
ax3.set_ylabel('average error (dB)')
ax3.set_xlabel('updates')
ax2.legend()
plt.savefig('H2P3_3c.png')
