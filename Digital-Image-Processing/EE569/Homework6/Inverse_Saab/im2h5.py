#
#  im2h5.py
#  EE569
#
#  Created by Alex on 2019/4/8.
#  Copyright 2019 Alex. All rights reserved.
#
#  combine input images and labels into a hdf5 file

import h5py
import numpy as np
import cv2
import os
import csv

data = []
label = []
with open('test_img.csv','rb') as csvfile:
    reader = csv.reader(csvfile)
    rows= [row for row in reader]

for row in rows:
    file = row[0]
    if os.path.isfile(file) == False:
        continue
    img = cv2.imread(file, 0)
    img = img.reshape(img.shape[0],img.shape[1],1)
    data.append(img)
    label.append(row[1])

with h5py.File('test_img.hdf5', 'w') as hf:
    hf.create_dataset('test_img', data = data)
    hf.create_dataset('test_label', data = label)

with h5py.File('test_img.hdf5', 'r') as hf:
    data = hf['test_img'][:]
    label = hf['test_label'][:]
print(data.shape)
print(label.shape)
