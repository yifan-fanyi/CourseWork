# Alex
# 2019.03.31
# audio_preprocessing.py
# input wav file generate MFCC audio data with label
# -*-conding:utf-8 -*-
import librosa
import csv
import os
import numpy as np
import h5py
import json

# csv file name in format: file_name label
csv_name = 'train.csv'
# length of each audio clips
data_length = 10
# number of clips from each audio
data_per_seq = 30
# output data
output_file = 'audio_data.hdf5'
# labels
dict = {'english': 0, 'hindi': 1, 'mandarin': 2}
#
train_test_split = 8
'''
# read csv by row
with open(csv_name,'rb') as csvfile:
    reader = csv.reader(csvfile)
    rows= [row for row in reader]
'''
# read json as dict
with open("train_files.json",'r') as js:
    js_dict = json.load(js, "UTF-8")

x_train = []
y_train = []
x_test = []
y_test = []
data_length *= 16000
count = 0

'''
# from csv
for row in rows:
    file = row[0]
    print(row)
    y, sr = librosa.load(file, sr=16000)
    for i in range(0, data_per_seq):
        start = np.random.randint(1, y.shape[0]-data_length-1, size=1)
        end = start[0] + data_length
        data = np.copy(y[int(start):int(end)])
        mat = librosa.feature.mfcc(y=data,
                                   sr=sr,
                                   n_mfcc=64,
                                   n_fft=int(sr*0.025),
                                   hop_length=int(sr*0.010))
        if count % train_test_split == 0:
            x_test.append(mat)
            y_test.append(dict[row[1]])
        else:
            x_train.append(mat)
            y_train.append(dict[row[1]])
        count += 1
'''

# from json
for na, id in js_dict.items():
    file = na
    if os.path.isfile(file) == False:
        continue
    print(na)
    y, sr = librosa.load(file, sr=16000)
    for i in range(0, data_per_seq):
        start = np.random.randint(1, y.shape[0]-data_length-1, size=1)
        end = start[0] + data_length
        data = np.copy(y[int(start):int(end)])
        mat = librosa.feature.mfcc(y=data,
                                   sr=sr,
                                   n_mfcc=64,
                                   n_fft=int(sr*0.025),
                                   hop_length=int(sr*0.010))
            #  if count % train_test_split == 0:
            #x_test.append(mat)
            #y_test.append(int(id))
            #  else:
        x_train.append(mat)
        y_train.append(int(id))
        count += 1

with h5py.File(output_file, 'w') as hf:
    hf.create_dataset('x_train', data = x_train)
    hf.create_dataset('y_train', data = y_train)
    #hf.create_dataset('x_test', data = x_test)
#hf.create_dataset('y_test', data = y_test)

# test
with h5py.File('audio_data.hdf5', 'r') as hf:
    print(hf.keys())
    x_train = hf['x_train'][:]
    y_train = hf['y_train'][:]
    #x_test = hf['x_test'][:]
    #y_test = hf['y_test'][:]
    print(x_train.shape)
    print(y_train.shape)
    #print(x_test.shape)
#print(y_test.shape)
