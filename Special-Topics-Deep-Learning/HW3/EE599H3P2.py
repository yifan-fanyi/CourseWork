#2019.02.05
#A 2 hidden layer MLP realization

import numpy as np
import h5py as h5

###########################Layer Defination################################################
class ReLU:
    def __init__(self):
        pass
    def foreward(self, Z):
        return np.maximum(Z, 0)

class tanh:
    def __init__(self):
        pass
    def foreward(self, Z):
        return (np.exp(Z)-np.exp(-Z))/(np.exp(Z)+np.exp(-Z))

class Softmax:
    def __init__(self):
        pass
    def foreward(self, Z):
        return np.exp(Z)/(np.sum(np.exp(Z),axis=0,keepdims=True)+0.00001)
    def backward(self, A, Y):
        return A - Y

class Dense:
    def __init__(self, input_node, output_node, activation, learning_rate, minbatch):
        self.input_node = input_node
        self.output_node = output_node
        self.W = np.random.randn(output_node, input_node)*np.sqrt(2)/np.sqrt(output_node+input_node)
        self.b = np.random.randn(output_node, 1)*np.sqrt(2)/np.sqrt(output_node+1)
        self.X = np.zeros((input_node, minbatch))
        self.activation = activation
        self.learning_rate = learning_rate
        self.minbatch = minbatch
    def foreward(self, X):
        self.X = X
        return np.dot(self.W, X)+self.b
    def backward(self, dZ):
        dx = np.copy(self.X)
        if self.activation == 1:
            dx[dx <= 0] = 0;
            dx[dx > 0] = 1;
        else:
            dx = 1 - tanh().foreward(dx)**2
        delta = dx*np.dot(self.W.T, dZ)
        self.W -= self.learning_rate*np.dot(dZ, self.X.T)/self.minbatch
        self.b -= self.learning_rate*np.mean(dZ, axis=1, keepdims=True)
        return delta

#############################model defination##############################################
class MLP:
    def __init__(self, input_node, num_classes, hidden1, hidden2, activation, decay, learning_rate, minbatch, epoch):
        self.Dense1 = Dense(input_node, hidden1, activation, learning_rate, minbatch)
        self.Dense2 = Dense(hidden1, hidden2, activation, learning_rate, minbatch)
        self.Dense3 = Dense(hidden2, num_classes, activation, learning_rate, minbatch)
        self.activation = activation
        self.decay = decay
        self.learning_rate = learning_rate
        self.minbatch = minbatch
        self.epoch = epoch
    def foreward(self, X):
        #foreward
        z = self.Dense1.foreward(X)
        if self.activation == 1:
            z = ReLU().foreward(z)
        else:
            z = tanh().foreward(z)
        z = self.Dense2.foreward(z)
        if self.activation == 1:
            z = ReLU().foreward(z)
        else:
            z = tanh().foreward(z)
        z = self.Dense3.foreward(z)
        z = Softmax().foreward(z)
        return z
    def backward(self, z, Y):
        #backward
        d = Softmax().backward(z, Y)
        d = self.Dense3.backward(d)
        d = self.Dense2.backward(d)
        d = self.Dense1.backward(d)
    def train(self, X, Y):
        z = self.foreward(X)
        self.backward(z, Y)
        return self.evaluate(z, Y)
    def test(self, X, Y):
        Z = self.foreward(X)
        return self.evaluate(Z, Y)
    def evaluate(self, A, Y):
        loss = -np.mean(np.sum(Y*np.log(A), axis=0))
        ypred = np.eye(10)[np.argmax(A, axis=0)].T
        acc = np.sum(ypred*Y)
        return loss, acc
    def saveParameter(self):
        hf = h5.File('myMLP.hdf5', 'w')
        if self.activation == 1:
            hf.attrs['act'] = np.string_("relu")
        else:
            hf.attrs['act'] = np.string_("tanh")
        hf.create_dataset('w1', data = self.Dense1.W)
        hf.create_dataset('b1', data = self.Dense1.b)
        hf.create_dataset('w2', data = self.Dense2.W)
        hf.create_dataset('b2', data = self.Dense2.b)
        hf.create_dataset('w3', data = self.Dense3.W)
        hf.create_dataset('b3', data = self.Dense3.b)
        hf.close()
    def loadModel(self, name):
        data = h5.File(name, 'r+')
        model.Dense1.b = data['b1'][:]
        model.Dense1.W = data['w1'][:]
        model.Dense2.b = data['b2'][:]
        model.Dense2.W = data['w2'][:]
        model.Dense3.b = data['b3'][:]
        model.Dense3.W = data['w3'][:]
        data.close()
    def fit(self, x_train, y_train, x_val, y_val):
        for j in range (0, self.epoch):
            i = 0
            loss_train = 0
            acc_train = 1
            while i < y_train.shape[0]:
                l, a = model.train(x_train[i: i+self.minbatch].T, y_train[i:i+self.minbatch].T)
                loss_train += l
                acc_train += a
                i = i+self.minbatch
            loss_val, acc_val = model.test(x_val.T, y_val.T)
            print("epoch: %3d/%d,  train loss: %7.5f,  train accuracy: %7.5f,  validation loss: %7.5f,  validation accuracy: %7.5f"%(j+1, epoch, loss_train/(y_train.shape[0]/self.minbatch), acc_train/y_train.shape[0], loss_val, acc_val/y_val.shape[0]))
            if self.decay == True and (j == 19 or j == 39):
                self.learning_rate /= 2
#################################mess up and separate data#################################
def Split(X, Y, N):
    P = np.random.permutation(X.shape[0])
    P_val = P[0:N]
    P_train = P[N:X.shape[0]]
    return X[P_train], X[P_val], Y[P_train], Y[P_val]

###############################read train data#############################################
data = h5.File('mnist_traindata.hdf5', 'r+')
X = data['xdata'][:]
Y = data['ydata'][:]
data.close()
############################fixed parameters###############################################
input_node = 784
num_classes = 10
############################parameters can be adjusted#####################################
#2 hidden layer
hidden1 = 200
hidden2 = 100

learning_rate = 0.1
minbatch = 100
epoch = 50
#decay = True: decay learning rate by 2 at 20th and 40th epoch
#decay = false: no decay
decay = True

#activation = 0: tanh
#activation = 1: relu
activation = 1
#number of validation samples
val_num = 10000

##############################train and save weight########################################
x_train, x_val, y_train, y_val = Split(X, Y, val_num)
model = MLP(input_node, num_classes, hidden1, hidden2, activation, decay, learning_rate, minbatch, epoch)
model.fit(x_train, y_train, x_val, y_val)
#model.loadModel('myMLP.hdf5')
model.saveParameter()
###############################test########################################################
data = h5.File('mnist_testdata.hdf5', 'r+')
x_test = data['xdata'][:]
y_test = data['ydata'][:]
data.close()
loss_test, acc_test = model.test(x_test.T, y_test.T)
print("test loss: %7.5f,  test accuracy: %7.5f"%(loss_test, acc_test/y_test.shape[0]))

