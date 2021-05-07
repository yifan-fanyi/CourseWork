# image to hdf5
# EE599H4
# 2019.02.28
# Yifan Wang

from __future__ import print_function
from keras.preprocessing import image
import numpy as np
from keras import backend as K
from keras.applications.imagenet_utils import preprocess_input
from keras_applications.imagenet_utils import _obtain_input_shape
from keras.engine.topology import get_source_inputs

import tensorflow as tf
import numpy as np
import os

dir="./train_image"

def getFileArr(dir):
    result_arr=[]
    label_list=[]
    map={}
    map_file_result={}
    map_file_label={}
    map_new={}
    count_label=0
    count=0
    
    file_list=os.listdir(dir)
    for file in file_list:
        file_path=os.path.join(dir,file)
        
        label=file.split(".")[0].split("_")[0]
        map[file]=label
        if label not in label_list:
            label_list.append(label)
            map_new[label]=count_label
            count_label=count_label+1
        
        img = image.load_img(file_path, target_size=(224, 224))
        x = image.img_to_array(img)
        x = preprocess_input(x)
        map_file_result[file]=x
        result_arr.append(x)
        count=count+1
            
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

    result=map_file_result[file]
    each_list.append(result)
    each_list.append(label)
    ret_arr.append(each_list)

    np.save('train_data.npy', ret_arr)
    
    return ret_arr

if __name__=="__main__":
    ret_arr=getFileArr(dir)
