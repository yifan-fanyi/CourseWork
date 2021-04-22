//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  Evaluate.hpp
//  EE569
//
//  Created by Alex on 2019/1/8.
//  Copyright © 2019 Alex. All rights reserved.
//
//  Functions that evaluate the work of my code

#ifndef Evaluate_hpp
#define Evaluate_hpp

#include "Basic.hpp"
using namespace std;

//SNR
double calSNR(C_c* org, C_c* pro, C_i& K, C_i& H, C_i& W);
double calSNR(C_Vec3_ui8& orgImg, C_Vec3_ui8& proImg);

//PSNR
double calPSNR(C_c* org, C_c* pro, C_i& K, C_i& H, C_i& W);
double calPSNR(C_Vec3_ui8& orgImg, C_Vec3_ui8& proImg);

//SSIM
double calSSIM(C_c* org, C_c* pro, C_i& K, C_i& H, C_i& W);
double calSSIM(C_Vec3_ui8& orgImg, C_Vec3_ui8& proImg);

#endif /* Evaluate_hpp */
