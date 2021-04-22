//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HalfToning.hpp
//  EE569
//
//  Created by Alex on 2019/1/24.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HalfToning_hpp
#define HalfToning_hpp

#include "HW2Basic.hpp"

//uniform random threshold
Vec3_ui8 Dithering(C_Vec3_ui8& Im3D);

//using index matrix
//Index: size of index matrix any number larger or equal to 2
//if Index is not the power of 2, it would shrink to k
//where k = 2^x < Index < 2^(x+1)
//th: adding uniform noise to IndexMatrix
//NoiseIndexMatrix = max(0,min(255,IndexMatrix+U(-abs(th),abs(th))));
//noise = 0; no noise
Vec3_ui8 DitheringMatrix(C_Vec3_ui8& Im3D, C_i& Index, C_i& noise);

//error diffusion
//version = 0: Floyd-Steinberg
//version = 1: Jarvis, Judice and Ninke (JJN)
//version = 2: Stucki
//Spread = 0: Raster parsing
//Spread = 1: Sperpentine parsing
//Spread = 2: Hibert curve
//color = true: convert to CMY and process as a color image
//color = false: convert to grayscale iamge process as a grayscale image
//threshold <= 0 no threshold on error
Vec3_ui8 ErrorDiffusion(C_Vec3_ui8& Im3D, C_i& version, C_i& spread, C_b& color, C_i& threshold);

//version = 0: Floyd-Steinberg
//version = 1: Jarvis, Judice and Ninke (JJN)
//version = 2: Stucki
//Sperpentine parsing
Vec3_ui8 MBVQErrDiff(C_Vec3_ui8& Im3D, C_i& version, C_i& threshold);

#endif /* HalfToning_hpp */
