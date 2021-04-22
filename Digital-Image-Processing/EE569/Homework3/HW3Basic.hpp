//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW3Basic.hpp
//  EE569
//
//  Created by Alex on 2019/2/4.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HW3Basic_hpp
#define HW3Basic_hpp

#include "Basic.hpp"
#include "HW1Basic.hpp"
#include "HW2Basic.hpp"

#include "GeometricModification.hpp"
#include "MorphologicalProcessing.hpp"
#include "HW3Fancy.hpp"

//detetor 4 corner ponits
//Method: basic part of FAST coner detector
//check 4 points in radius 3 near central pixel
//image edge do not include
//threshold: differ from center pixel
Vec2_i DetectCorner(C_Vec2_ui8& Im2D, C_i& threshold);

//supress not wanted corners
//specific for condition like lightthouse1.raw
Vec2_i SelectCorner(C_Vec2_i& Corners);

//specificl for cases in lighthouse 1;
Vec2_i DetectCornerHole(C_Vec2_ui8& Im2D);

//calculate simality between patch and hole to decide wether rotate or not
//return: # need to rotate clockwise
int calHoleFillScore(C_Vec2_ui8& Im2D_1, C_Vec2_ui8& Im2D_2, C_i& x1, C_i& y1, C_i& len);

//Gaussian Newton regressor
//for distortion correction with k1=-0.3536, k2=0.1730
//#iter = 20
void NewtonMethod(double& xDistortion, double& yDistortion, double& xCorrected, double& yCorrected);

#endif /* HW3Basic_hpp */
