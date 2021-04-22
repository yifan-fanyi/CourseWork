//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  Basic.hpp
//  HEE569
//
//  Created by Alex on 2019/1/7.
//  Copyright © 2019 Alex. All rights reserved.
//
//  This file include the basic reading and writing functions
//  and matrix operations

#ifndef Basic_hpp
#define Basic_hpp

#include "core.hpp"

#include "Matrix.hpp"
#include "ImgBasic.hpp"

#include "Evaluate/Evaluate.hpp"
#include "BinaryTree/BinaryTree.hpp"
#include "KMeans/KMeans.hpp"

using namespace std;
using namespace cv;

//read raw to 3D vector
Vec3_ui8 ReadRaw(C_c* Path, C_i& K, C_i& H, C_i& W);

//write to file
void WriteRaw(C_c* Path, C_Vec3_ui8& Img3D);
void WriteRaw(C_c* Path, C_Vec2_ui8& Img2D);
void WriteRaw(C_c* Path, C_Vec2_i& Img2D);
void WriteRaw(C_c* Path, C_Vec2_d& Img2D);
void WriteRaw(C_c* Path, C_Vec3_i& Img3D);
void WriteRaw(C_c* Path, C_Vec3_d& Img3D);

//seperate string by ' '
Vec1_s separate(C_s& s);

//cv::mat to raw
//all in color
//mat BGR to raw RGB
Vec3_ui8 Mat2Raw(C_c* in_Path);

//raw to cv::mat
//raw RGB to mat BGR
cv::Mat Raw2Mat(C_Vec3_ui8& Img3D, C_c* out_Path);
cv::Mat Raw2Mat(C_Vec3_i& Img3D, C_c* out_Path);
cv::Mat Raw2Mat(C_Vec3_d& Img3D, C_c* out_Path);
cv::Mat Raw2Mat(C_Vec2_ui8& Img2D, C_c* out_Path);
cv::Mat Raw2Mat(C_Vec2_i& Img2D, C_c* out_Path);
cv::Mat Raw2Mat(C_Vec2_d& Img2D, C_c* out_Path);
cv::Mat Raw2Mat(C_c* in_Path, C_c* out_Path, C_i& K, C_i& H, C_i& W);

#endif /* Basic_hpp */
