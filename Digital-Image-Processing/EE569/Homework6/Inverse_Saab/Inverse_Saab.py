#
#  Inverse_Saab.py
#  EE569
#
#  Created by Alex on 2019/4/9.
#  Copyright 2019 Alex. All rights reserved.
#
#  inverse transfer of 2 stage 4*4 saab transforation with stride 4

import pickle
import numpy as np
import cv2
from numpy import linalg as LA
from skimage.util.shape import view_as_windows
import data
from tensorflow.python.platform import flags
from Getkernel_compact import GetKernel


flags.DEFINE_string("useclasses", "0-9", "Supported format: 0,1,5-9")
F = flags.FLAGS

#  number of kernel each stage
s1 = 6
s2 = 16
#  run Getkernel_compact.py first to get .pkl
'''
train_images, train_labels, test_images, test_labels, class_list = data.import_data(F.useclasses)
GetKernel(train_images, train_labels, class_list,
          './weight/pca_params_compact_inv.pkl')
'''
fr = open('./weight/pca_params_compact_inv.pkl', 'rb')
pca_params = pickle.load(fr)
fr.close()

k0 = np.array(pca_params['Layer_0/kernel'])
k1 = np.array(pca_params['Layer_1/kernel'])

b1 = pca_params['Layer_1/bias'].astype(np.float32)

#  read image and get features
img = cv2.imread('4.png',0)
patch = view_as_windows(img,(4, 4),step=(4, 4)).reshape(8,8,1*4*4)
patch = np.dot(patch, np.transpose(k0))
patch = view_as_windows(patch.copy(),(4,4,1),step=(4,4,1))
patch = patch.reshape(2,2,s1*16)
patch = patch + 1 / np.sqrt(s1*s2) * b1
patch = np.dot(patch, np.transpose(k1))

#  inverse transforation
ipatch = np.dot(patch, LA.pinv(np.transpose(k1)))
ipatch = ipatch - 1 / np.sqrt(s1*s2) * b1
ipatch = ipatch.reshape(2,2,s1,16)
ipatch = np.moveaxis(ipatch,2,3)
xx = np.zeros((8,8,s1))
for i in range(0,2):
    for j in range(0,2):
        for k in range(0,16):
            xx[i*4+k//4,j*4+k%4]=ipatch[i,j,k]
ipatch = xx
ipatch = np.dot(ipatch, LA.pinv(np.transpose(k0)))
xx = np.zeros((32,32))
for i in range(0,8):
    for j in range(0,8):
        for k in range(0,16):
            xx[i*4+k//4,j*4+k%4]=ipatch[i,j,k]
ipatch = 254*(xx-np.min(xx))/(np.max(xx)-np.min(xx))

#  save image
cv2.imwrite('_%d_%d.jpg'%(s1,s2),ipatch)

#  output PSNR
mse = np.mean(np.square(xx-img))
psnr =10*np.log10(255**2/mse)
print(psnr)

'''
#  two stage inverse PCA with kernel 4*4 stride 4
#  it would be the same using one stage PCA with kernel 16*16 stride 16

import numpy as np
import cv2
from numpy import linalg as LA
from sklearn.decomposition import PCA
from skimage.util.shape import view_as_windows
import keras
from keras.datasets import mnist
import h5py

keep_c = 32
(train_images, train_labels), (test_images, test_labels) = mnist.load_data()
train_images = train_images.reshape(-1,28,28,1)
train_images = train_images/255.
train_images = np.float32(train_images)

train_images = np.pad(train_images, ((0,0),(2,2),(2,2),(0,0)), mode='constant')
train_images = np.moveaxis(train_images, 3,1)
patches1=view_as_windows(train_images.copy(), (1, 1, 16, 16), step=(1, 1, 16, 16))
patches1 = patches1.reshape(60000*4,256)
patches1 = patches1 - np.mean(patches1,axis=1, keepdims=True)


pca = PCA(n_components=keep_c, svd_solver='full')
pca.fit(patches1)
kernels = pca.components_[:keep_c, :]


with h5py.File('test_img.hdf5', 'r') as hf:
    test_img = hf['test_img'][:]
    test_label = hf['test_label'][:]
#test_img = test_img/255
test_img = np.float32(test_img)
test_img = np.moveaxis(test_img, 3,1)
patches = view_as_windows(test_img.copy(), (1, 1, 16, 16), step=(1, 1, 16, 16))
patches = patches.reshape(test_img.shape[0]*4,256)
patches = patches - np.mean(patches,axis=1, keepdims=True)


x = np.dot(patches, np.transpose(kernels))
x = np.dot(x, LA.pinv(np.transpose(kernels)))


xx = np.zeros((32,32))
for i in range(0,patches.shape[0]):
    for j in range(0,256):
        xx[((i%4)//2)*16+j//16,((i%4)%2)*16+(j%16)]=x[i,j]
    if i%4 == 3:
        xx = 255*(xx-np.min(xx))/(np.max(xx)-np.min(xx))
        cv2.imwrite('inverse_%s_%d.jpg'%(test_label[i//4],keep_c),xx)
        xx = np.zeros((32,32))
'''

