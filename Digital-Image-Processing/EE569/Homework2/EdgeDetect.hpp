//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  EdgeDetect.hpp
//  EE569
//
//  Created by Alex on 2019/1/21.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef EdgeDetect_hpp
#define EdgeDetect_hpp

#include "HW2Basic.hpp"

//rate: rate of top gradients to keep
Vec3_ui8 SobelED(C_Vec3_ui8& Im3D, C_d& keep_rate);

//test Canny using openCV canny
//in_Path: input jpeg
//out_Path: output jpeg
void testCanny(C_c* in_Path, C_c* out_Path, C_i& low_threshold, C_i& high_threshold, C_i& kernel);

#endif /* EdgeDetect_hpp */
