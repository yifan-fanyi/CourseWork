//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW1Fancy.hpp
//  EE569
//
//  Created by Alex on 2019/1/25.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HW1Fancy_hpp
#define HW1Fancy_hpp

#include "HW1Basic.hpp"

Vec3_ui8 HW1Fancy1();

//image seg
Vec3_ui8 Seg(C_Vec3_ui8& Im3D, C_d& sigma, C_i& search_window, C_i& compare_window);

//medium filter
Vec3_ui8 MedFilter(C_Vec3_ui8& Im3D, C_i& kernel);

//most accurate version
Vec3_ui8 BiDemosaicing(C_Vec3_ui8& Im3D);

//偏黄，但我也找不出问题在哪也懒得找了， tmp[3]处可能有问题
Vec3_ui8 MHCDemosaicingC(C_Vec3_ui8& Im3D);

//unrolling a pixel to 2*2 pixels
Vec3_ui8 Unrolling3D(C_Vec3_ui8& Im3D);

//one sigma only
Vec3_ui8 BilateralFilter1(C_Vec3_ui8& Im3D, C_i& kernel_size, C_d& sigma);
#endif /* HW1Fancy_hpp */
