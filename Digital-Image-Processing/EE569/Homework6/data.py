#  from https://github.com/davidsonic/Interpretable_CNN
#  add some function by Alex
#  2019.04.16
#

import keras
from keras.datasets import mnist
import numpy as np
import saab_compact
import cv2
from keras.models import load_model

def get_data_for_class(images, labels, cls):
	if type(cls)==list:
		idx=np.zeros(labels.shape, dtype=bool)
		for c in cls:
			idx=np.logical_or(idx, labels==c)
	else:
		idx=(labels==cls)
	return images[idx], labels[idx]

def import_data(use_classes):
	(train_images, train_labels), (test_images, test_labels) = mnist.load_data()
	train_images=train_images.reshape(-1,28,28,1)
	test_images=test_images.reshape(-1,28,28,1)
	train_images=train_images/255.
	test_images=test_images/255.

	train_images=np.float32(train_images)
	test_images=np.float32(test_images)

	print 'initiali dtype: ', train_images.dtype
	# print(train_images.shape) # 60000*28*28*1

	# zeropadding
	train_images=np.pad(train_images, ((0,0),(2,2),(2,2),(0,0)), mode='constant')
	test_images=np.pad(test_images,  ((0,0),(2,2),(2,2),(0,0)), mode='constant')
	# print(train_images.shape) # 60000*32*32*1

	if use_classes!='0-9':
		class_list=saab.parse_list_string(use_classes)
		train_images, train_labels=get_data_for_class(train_images, train_labels, class_list)
		test_images, test_labels=get_data_for_class(test_images, test_labels, class_list)
		# print(class_list)
	else:
		class_list=[0,1,2,3,4,5,6,7,8,9]
		
	return train_images, train_labels, test_images, test_labels, class_list

def LawsFilter(x_train):
    filter = np.array([[1, 4, 6, 4, 1],
                       [-1, -2, 0, 2, 1],
                       [-1, 0, 2, 0, -1],
                       [-1, 2, 0, -2, 1],
                       [1, -4, 6, -4, 1]])
    kernel = []
    for i in range(0,2):
        for j in range(0,5):
            kernel.append(np.dot(filter[i].reshape(5,1), filter[j].reshape(1,5)).reshape(5,5))
    kernel = np.array(kernel)
    data = []
    data.append(x_train)
    for i in range(1,10):
        tmp = np.zeros_like(x_train)
        for k in range(0,x_train.shape[0]):
            tmp[k] = cv2.filter2D(x_train[k], -1, kernel[i]).reshape(28,28)
        data.append(tmp)
    data = np.array(data).reshape(10,-1,28,28,1)
    return data

def SeparateDate(x_train, y_train):
    count = np.zeros((10))
    idx = np.array([0,5000,10000,15000,20000,
                    25000,30000,35000,40000,45000])
    x_train_sep = np.zeros_like(x_train)
    y_train_sep = np.zeros_like(y_train)
    for i in range(0,x_train.shape[0]):
        if count[y_train[i]] > 4999:
            continue
        t = int(count[y_train[i]]//500)
        x_train_sep[idx[t]] = x_train[i]
        y_train_sep[idx[t]] = y_train[i]
        idx[t] += 1
        count[y_train[i]] += 1
    return x_train_sep, y_train_sep

def CompOverlap(ypred, y_test, x_test):
    bp_cnn = load_model('LeNet5_trained.h5')

    bp_pred = bp_cnn.predict(x_test)
    bp_pred = np.eye(10)[np.argmax(bp_pred, axis=1)]

    overlap_bp = bp_pred - ypred
    overlap_ff = ypred - bp_pred

    overlap_bp[overlap_bp < 0] = 0
    overlap_ff[overlap_ff < 0] = 0

    print('number of different pred: %5d'%(np.sum(overlap_bp)))
    print('acc in BP of different pred data: %5f'%(np.sum(overlap_bp*y_test)/np.sum(overlap_bp)))
    print('acc in FF of different pred data: %5f'%(np.sum(overlap_ff*y_test)/np.sum(overlap_ff)))

def MNIST_GP():
    (x_train, y_train), (x_test, y_test) = mnist.load_data()
    x_train = x_train.reshape(x_train.shape[0], 28, 28, 1)
    x_test = x_test.reshape(x_test.shape[0], 28, 28, 1)
    x_train = x_train.astype('float32')
    x_test = x_test.astype('float32')
    x_train /= 255
    x_test /= 255
    return x_train, y_train, x_test, y_test
