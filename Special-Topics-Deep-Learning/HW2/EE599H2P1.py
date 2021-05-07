#2019.01.25
import numpy as np
import h5py as h5

def testMLP(x):
    f = h5.File('mnist_network_params.hdf5', 'r')
    w1 = f['W1'][:]
    b1 = f['b1'][:].reshape(-1, 1)
    w2 = f['W2'][:]
    b2 = f['b2'][:].reshape(-1, 1)
    w3 = f['W3'][:]
    b3 = f['b3'][:].reshape(-1, 1)
    f.close()
    x = np.dot(w1,x)+b1
    x = ReLU(x)
    x = np.dot(w2,x)+b2
    x = ReLU(x)
    x = np.dot(w3,x)+b3
    y = Softmax(x)
    ypred = Softmax2Onehot(y)
    return ypred, y

def ReLU(x):
    return np.maximum(x, 0)

def Softmax(x):
    return np.exp(x)/np.sum(np.exp(x),axis=0)

def Softmax2Onehot(x):
    loc = np.argmax(x, axis=0)
    res = np.eye(10)[loc]
    return res

def testAcc(x):
    ypred, yact = testMLP(x.T)
    return ypred, yact

data = h5.File('mnist_testdata.hdf5', 'r+')
test_x = data['xdata'][:]
data.close()

ypred, yact = testAcc(test_x)
ylabel = np.argmax(ypred,axis=1)

output = []
for idx in range(0, len(test_x)):
    output.append({
                  "activations": yact[:,idx].tolist(),
                  "index": idx,
                  "classification": int(ylabel[idx])
                  })
import json
print("AUTOGRADE: %s"%(json.dumps(output)))

