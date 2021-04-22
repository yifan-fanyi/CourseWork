#  from https://github.com/davidsonic/Interpretable_CNN

import pickle
import numpy as np
import data
import saab_compact as saab
import keras
import sklearn

def GetFeature(name_kernel,name_feature, train_images):
    # load data
    fr = open(name_kernel, 'rb')
    pca_params = pickle.load(fr)
    fr.close()

    # Training
    print('--------Training--------')
    train_images=np.moveaxis(train_images, 3, 1)
    feature = saab.initialize(train_images, pca_params)

    feature = feature.reshape(feature.shape[0], -1)
    print("S4 shape:", feature.shape)
    print('--------Finish Feature Extraction subnet--------')
    print 'feature.dtype: {}'.format(feature.dtype)
    feat = {}
    feat['feature'] = feature
    
    # save data
    fw = open(name_feature, 'wb')
    pickle.dump(feat, fw)
    fw.close()


