# Alex 
# 2019.04.11
# improved AtrousFCN_Resnet50_16s with edges
# add a edge label to as a additional loss
# by modifying the loss_function.py in 'utils'
# usage: suppose the code is put in ~/src


# tensorflow/tensorflow-gpu 1.4.0
# keras 2.1.2
# cuda 8.0
# cudnn 6.0

# AWS Deep Learning Ubuntu 14.04
'''
# pip
curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
sudo python get-pip.py
'''

nvidia-smi 
# cuda
wget https://developer.nvidia.com/compute/cuda/8.0/Prod2/local_installers/cuda_8.0.61_375.26_linux-run
sudo sh cuda_8.0.61_375.26_linux-run --override --silent --toolkit 

# cudnn
tar -xzvf cudnn-8.0-linux-x64-v6.0.tgz
sudo cp cuda/include/cudnn.h /usr/local/cuda/include
sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64
sudo chmod a+r /usr/local/cuda/include/cudnn.h /usr/local/cuda/lib64/libcudnn*


# usage:
mkdir -p ~/src

# install dependencies
cd ~/src
sudo pip install pillow keras sacred

# fork of keras-contrib necessary for DenseNet based models
git clone https://github.com/ahundt/keras-contrib.git -b densenet-atrous
cd keras-contrib
sudo python setup.py install

cd ~/src
git clone https://github.com/ahundt/tf-image-segmentation.git -b Keras-FCN

# Automated Pascal VOC Setup (recommended)
export PYTHONPATH=$PYTHONPATH:~/src/tf-image-segmentation
cd ~/src/tf-image-segmentation/tf_image_segmentation/recipes/pascal_voc/
python data_pascal_voc.py pascal_voc_setup



# Generate pretrained weights
cd ~/src/Keras-FCN
cd utils
python transfer_FCN.py ResNet50


# Run training
cd ~/src/Keras-FCN
python train.py

# Evaluate the performance of the network
python evaluate.py