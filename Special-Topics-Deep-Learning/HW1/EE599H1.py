#2019.01.11
import scipy.signal as signal
import numpy as np
import matplotlib.pyplot as plt
pi = 3.1415926535

##draw freq response
alpha = np.array([-0.5, 0.1, 0.5, 0.9])
for index in range (0,4):
    a = np.array([1.0, -alpha[index]])
    b = np.array([1-alpha[index], 0])
    #save figure separatly
    w, h= signal.freqz(b,a,whole='true')
    fig = plt.figure(figsize = (5,5))
    ax1 = plt.subplot()
    ax1.set_title('alpha = '+str(alpha[index]))
    ax1.plot((w-pi)/(2*pi), 20 * np.log10(abs(h)))
    ax1.set_ylabel('Amplitude [dB]')
    ax1.set_xlabel('Frequency [cycles/sample]')
    plt.savefig('result_'+str(alpha[index])+'.png')


###draw 20% decay
for index in range (1,4):
    x = np.arange(0,20)
    y = (1-alpha[index])*alpha[index]**x
    m = np.where(y < 0.2*y.max())
    print('alpha = ',alpha[index], '20% decay point is', x[m[0][0]])
    fig = plt.figure(figsize = (7,7))
    ax2 = plt.axes()
    ax2.set_title('alpha = '+str(alpha[index]))
    for i in range (0,20):
        ax2.arrow(x[i], 0, 0, y[i], head_width=0, head_length=0)
    ax2.plot(x[m[0]], y[m[0]], 'ro')
    ax2.set_ylim(0,1.1-alpha[index])
    ax2.set_xlim(-1,20)
    ax2.set_ylabel('Amplitude')
    ax2.set_xlabel('n')
    plt.savefig('result_1'+str(alpha[index])+'.png')

##generate butterworth L = 4
b, a = signal.butter(4, 0.2, 'lowpass')
w, h = signal.freqz(b, a)

fig = plt.figure(figsize = (5,5))
ax1 = plt.subplot()
ax1.set_title('Butterworth L = 4')
ax1.plot((w-pi)/(2*pi), 20 * np.log10(abs(h)))
ax1.set_ylabel('Amplitude [dB]')
ax1.set_xlabel('Frequency [cycles/sample]')
plt.savefig('butter.png')

print('a: ',a)
print('b: ',b)

np.random.seed(0)
s = np.random.normal(0, 1, 300)
fig = plt.figure(figsize = (5,5))
y = signal.lfilter(b,a,s)
ax2 = plt.subplot()
ax2.set_title('Butterworth on Unit Gaussian')
ax2.plot(np.arange(300), s, label='input')
ax2.plot(np.arange(300), y, "ro",label='output')
ax2.legend()
plt.savefig('butterresult.png')


