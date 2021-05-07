import os
import numpy as np
from PIL import Image
import tensorflow as tf
import sklearn
from sklearn import preprocessing
import csv

import shutil

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
#test samples for each class
a = np.array([30,300,300,30,30,30,8,2])
b = np.zeros(8)

with open('train.csv','rb') as csvfile:
    reader = csv.reader(csvfile)
    rows= [row for row in reader]

dict = {'disgust':'disgust',
        'neutral':'neutral',
        'happiness':'happiness',
        'surprise':'surprise',
        'anger':'anger',
        'sadness':'sadness',
        'fear':'fear',
        'contempt':'contempt'}
num = 0
count = 0
label = np.zeros((len(rows)))
for r in rows:
    if r[1] not in dict:
        print(r[1], num)
        dict[r[1]] = num
        num += 1
    
    label[count] = dict[r[1]]
    count += 1

file_dir = '.'

count = 0;
os.mkdir('./data')
os.mkdir('./data/train')
for i in dict:
    os.mkdir('./data/train/'+str(i))
os.mkdir('./data/val')
for i in dict:
    os.mkdir('./data/val/'+str(i))

list_dir = os.listdir(file_dir+'/train_image')
list_dir = sorted(list_dir)


for file in list_dir:
    print(file)
    if file == '.DS_Store':
        continue
    i = int(label[count])
    if b[i] < a[i]:
        b[i] += 1
        shutil.move(file_dir+'/train_image/'+file,'./data/val/'+str(i))
    else:
        shutil.move(file_dir+'/train_image/'+file,'./data/train/'+str(i))
    count += 1






