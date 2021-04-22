//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW3Fancy.hpp
//  EE569
//
//  Created by Alex on 2019/2/13.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HW3Fancy_hpp
#define HW3Fancy_hpp

#include "HW3Basic.hpp"

void DeerFancy();

//using other features like color length
void RiceSizeFancy(C_Vec3_ui8& Im3D, C_i& Nclass);

//length of a rice
Vec1_d calRiceLength(C_Vec3_i& loc);

Vec2_d KmeansInit1(C_Vec2_d& data, C_i& N, C_i& dim);

void KmeansCluster1(C_Vec2_d& data, Vec2_d& center, Vec1_i& label, C_i& dim, bool& modified);

Vec1_i Kmeans1(C_Vec2_d& loc, C_i& N, C_i& dim);

//mapping back using iterative method
Vec2_ui8 SpatialWarping2DBack(C_Vec2_ui8& Im2D, C_Vec2_d& coef);

Vec2_d GetTriplets(C_i& H, C_i& W);

Vec2_ui8 T(C_Vec2_ui8& Im2D);

#endif /* HW3Fancy_hpp */
