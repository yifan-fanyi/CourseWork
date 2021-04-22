# update on 2019.04.21.01.43
# Alex
# yifanwang0916@outlook.com

# for Homework 1-4
# environment:
	matlab: 
		HW1 shot noise
			provided code is enough
			from http://www.cs.tut.fi/~foi/GCF-BM3D/
		HW2 structure edge
			matlab 2016+ may not work
			need toolbox from https://github.com/pdollar/edges
	C++11:
		all problems expect above 2 in matlab 
		requirement:
 			OpenCV
		written in Xcode on macOS High Sierra(10.13.6)

# cmakelist.txt usage: 
 	get into project folder
 		mkdir build
 		cd build
 		cmake ..
 		make -j8

# 	Then an executable file named 'run' is generated in project folder 
# 	put you test image in '/data'
 		./run
# 	result would be saved to '/data' as well

# main.cpp is just used for testing

# for Homework 5 & 6
# environment:
 	python 2.7
 	Keras 2.1.2 with tensorflow 1.9.0 as backend

# Homework 6
# modified from https://github.com/davidsonic/Interpretable_CNN
# inverse_saab deal with 32*32 mnist with kernel 4*4 stride 4

 
