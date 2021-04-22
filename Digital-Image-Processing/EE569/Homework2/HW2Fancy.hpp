//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW2Fancy.hpp
//  EE569
//
//  Created by Alex on 2019/1/28.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HW2Fancy_hpp
#define HW2Fancy_hpp

#include "HW2Basic.hpp"

#include "../ThirdParty/HilbertCurve/HilbertCurve.hpp"

//window = 2 is good
//method = 0: DitheringMatrix
//method = 1: Error Diffusion (1,1), color = true, th = -1;
//method = 2: MBVQ based Error Diffusion (1,1), th = -1;
Vec3_ui8 HW2Fancy1(C_Vec3_ui8& Im3D, C_i& window, C_i& method);

//Hilbert Sweep;
//not optimized
Vec2_ui8 HW2Fancy2(C_Vec2_ui8& Im3D, C_i& version);

//generate random sequence, For HW2Fancy1
Vec1_i genRandomSeq(C_i& length);

//mess the result from dithering matrix I32_-1, For HW2Fancy1
Vec3_ui8 Mess(C_Vec3_ui8& Im3D, C_i& window);

//useless
Vec3_ui8 Stack3D(C_Vec3_ui8& Im3D);

//use recursive to get index matrix
void runGetIndex(int n, Vec2_i& mat);
Vec2_i getIndexRec(int n);

#endif /* HW2Fancy_hpp */
