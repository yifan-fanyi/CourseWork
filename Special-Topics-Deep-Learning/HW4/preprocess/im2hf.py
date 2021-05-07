# image to hdf5
# EE599H4
# 2019.02.27
# Yifan Wang

import os
import numpy as np
from PIL import Image
import tensorflow as tf
import matplotlib.pyplot as plt
import sklearn
from sklearn import preprocessing
import h5py
import scipy
import csv
import cv2

'''
    ('disgust', 0)
    ('neutral', 1)
    ('happiness', 2)
    ('surprise', 3)
    ('anger', 4)
    ('sadness', 5)
    ('fear', 6)
    ('contempt', 7)
'''

with open('train.csv','rb') as csvfile:
    reader = csv.reader(csvfile)
    rows= [row for row in reader]

dict = {}
num = 0
count = 0
print(len(rows))
label = np.zeros((len(rows)))
for r in rows:
    if r[1] not in dict:
        print(r[1], num)
        dict[r[1]] = num
        num += 1
    
    label[count] = dict[r[1]]
    count += 1

file_dir = '.'

HEIGHT = 224
WIDTH = 224
CHANNELS = 3
SHAPE = (HEIGHT, WIDTH, CHANNELS)

image = []
list_dir = os.listdir(file_dir+'/train_image')
list_dir = sorted(list_dir)
for file in list_dir:
   
    if file == '.DS_Store':
        continue
    print(file)
    data = cv2.imread(file_dir +'/train_image'+'/'+file)

    data = cv2.resize(data, (WIDTH,HEIGHT), interpolation=cv2.INTER_CUBIC)
    image.append(data)


imagedata = np.array(image)
#print(image)
with h5py.File('emotion.hdf5', 'w') as hf:
    hf.create_dataset('emotion', data = imagedata)
    hf.create_dataset('label', data = label)

#test
with h5py.File('emotion.hdf5', 'r+') as hf:
    print(hf.keys())
    tdata = hf['emotion'][:]
    tlabel = hf['label'][:]

print(tdata.shape)
print(tlabel.shape)

cv2.imwrite('x.jpg',tdata[10])
print(tlabel[10])
np.testing.assert_array_equal(tlabel, label)
np.testing.assert_array_equal(tdata, imagedata)
