#2019.01.25
import numpy as np
import h5py as h5
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression
from sklearn.cross_validation import train_test_split, cross_val_score
from sklearn.preprocessing import StandardScaler
from numpy.linalg import svd, eig, inv

def plot_heat_map(title, data):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    cax = ax.matshow(data, origin='lower', cmap='rainbow', aspect='auto')
    fig.colorbar(cax)
    ax.xaxis.set_ticks_position('bottom')
    plt.title(title)
    plt.show()

#read
f = h5.File('binary_random_sp2019.hdf5', 'r')
human = f['human'][:]
machine = f['machine'][:]
f.close

human = (-1) ** human
machine = (-1) ** machine

#setlabel and concat
y_human = -1*np.ones(human.shape[0])
y_machine = np.ones(machine.shape[0])
x = np.concatenate((human, machine), axis=0)
y = np.concatenate((y_human, y_machine), axis=0)
#cal R
R_human = np.dot(human.T, human)
R_machine = np.dot(machine.T, machine)
R = np.dot(x.T, x)
#plot_heat_map('R_human', R_human[::-1] )
#plot_heat_map('R_machine', R_machine[::-1] )
#plot_heat_map('R_total', R[::-1] )

#cal eigen value
eValue_h,eVector_h = eig(R_human)
eValue_m,eVector_m = eig(R_machine)
eValue_R,eVector_R = eig(R)
print('R_human eValue: ', eValue_h[0:2])
print('R_human percentage of variance: ', np.sum(eValue_h[0:2])/np.sum(eValue_h))
print('R_machine eValue: ', eValue_m[0:2])
print('R_machine percentage of variance: ', np.sum(eValue_m[0:2])/np.sum(eValue_m))
fig = plt.figure()
plt.stem(eValue_R, label = 'total', linefmt = 'tab:gray', markerfmt='ko')
axes = plt.gca()
axes.set_xlabel('index (k)')
axes.set_ylabel('eigen-vlaues of R matrix')

fig = plt.figure()
plt.stem(eValue_h, label = 'total', linefmt = 'tab:gray', markerfmt='ko')
axes = plt.gca()
axes.set_xlabel('index (k)')
axes.set_ylabel('eigen-vlaues of R-human matrix')

fig = plt.figure()
plt.stem(eValue_m, label = 'total', linefmt = 'tab:gray', markerfmt='ko')
axes = plt.gca()
axes.set_xlabel('index (k)')
axes.set_ylabel('eigen-vlaues of R-machine matrix')

#project to 2D
e_h = eVector_h[0:2]
e_m = eVector_m[0:2]
e_R = eVector_R[0:2]
human_proj = np.dot(human, e_h.T)
machine_proj = np.dot(machine, e_m.T)
x_proj = np.dot(x, e_R.T)

fig = plt.figure()
ax1 = plt.subplot()
ax1.plot(human_proj[1:200,0], human_proj[1:200,1], 'ro', label = 'human')
ax1.plot(machine_proj[1:200,0], machine_proj[1:200,1], 'bo', label='machine')


w, Re, rank, singular_vals = np.linalg.lstsq(x, y, rcond=None)

y_hat_soft = np.dot(w , x.T)
y_hat_hard = np.sign( y_hat_soft )
prob_mc = np.mean( y != y_hat_hard )
#print('mis classification rate: ', prob_mc)
#print('\n\n\n\n')

#proj w
w_proj = np.dot(w.reshape(1,20), e_R.T)
#print(w_proj.shape)
#print(np.dot(w.reshape(1,20), e_R.T).shape)
p = np.arange(-2,2,0.01)

#ax1.plot(p, w_proj[0,0]+w_proj[0,1]*p, label='seperate line')
ax1.legend()
ax1.set_xlabel('first feature')
ax1.set_ylabel('second feature')
plt.show()

#print(w_proj)
