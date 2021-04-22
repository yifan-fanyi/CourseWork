//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW1Basic.hpp
//  EE569
//
//  Created by Alex on 2019/1/25.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HW1Basic_hpp
#define HW1Basic_hpp

#include "Basic.hpp"

#include "ImgDemHistMupl.hpp"
#include "ImageDenoising.hpp"
#include "HW1Fancy.hpp"

//save result from getHistogram
void save2DHistVec(C_Vec2_i& db, C_c* name, int K, int H, int W);

void save2DCumHistVec(C_Vec2_i& db, C_c* name, int K, int H, int W);

//draw and save histogram to 300*300 image
void saveHistogram(const Vec3_ui8& Im3D, C_c* out_Path);

//histogran equalization method B
Vec2_ui8 HistEqMethodB2D(C_Vec2_ui8& Im2D);

//core for gaussian filter
Vec2_d genGaussianCore(C_i& kernel_size, C_d& sigma);

//enlarge = false, return exp(-((i-ii)^2+(j-jj)^2)/sigma^2)
//enlarge = true, return exp(-((i-ii)^2+(j-jj)^2)/(sigma^2*1000))
float genGaussianCe(C_d& sigma, C_i& i, C_i& j, C_i& ii, C_i& jj, C_b& enlarge);

//pixel val distance
float genSimilarityCe(C_Vec3_ui8& Im3D, C_d& sigma, C_i& i, C_i& j, C_i& ii, C_i& jj);

template<class T>
double calColorDist(const T& oR, const T& oG, const T& oB, const T& xR, const T& xG, const T& xB){
    double r ((oR+xR)/2);
    double dR2 (Sq(oR-xR));
    double dG2 (Sq(oG-xG));
    double dB2 (Sq(oB-xB));
    return (2+r/256)*dR2+4*dG2+(2+(255-r)/256)*dB2;
}

//Anscombe transformtion
double AnscombeRT(C_ui8& val);

//invese Anscombe transformtion
//no bias version
uint8_t IAnscombeRT(C_d& val);

#endif /* HW1Basic_hpp */
