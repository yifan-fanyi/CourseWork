//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  ImgDemHistMupl.hpp
//  EE569
//
//  Created by Alex on 2019/1/7.
//  Copyright © 2019 Alex. All rights reserved.
//
//  This file include image processing algorthim that required in homework

#ifndef ImgDemHistMupl_hpp
#define ImgDemHistMupl_hpp

#include "HW1Basic.hpp"

//bilinear demosaicing
Vec3_ui8 BiDemosaicingC(C_Vec3_ui8& Im3D);

//MHC demosaicing
Vec3_ui8 MHCDemosaicing(C_Vec3_ui8& Im3D);

//CDF based histogram equalization method
Vec3_ui8 HistEqMethodA(C_Vec3_ui8& Im3D);

//average distribut histogram equalization method
Vec3_ui8 HistEqMethodB(C_Vec3_ui8& Im3D);

#endif /* ImgDemHistMupl_hpp */
