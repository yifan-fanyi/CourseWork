//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  GeometricModification.hpp
//  EE569
//
//  Created by Alex on 2019/2/11.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef GeometricModification_hpp
#define GeometricModification_hpp

#include "HW3Basic.hpp"

//using vertical mean for horizontal edge vice versa to smooth edges
void SmoothSingleDir(Vec2_ui8& Im2D_1, C_Vec2_i& loc, C_i len2);

//processing a patch each single time
//patch is supposed to be square
//rotate to vertical
//scaling to fit hole size
//calculate simality to decide if rotate
//assemble patch
void GeoTransform2D(Vec2_ui8& Im2D_1, C_Vec2_ui8& Im2D_2, C_Vec2_i& loc);

Vec3_ui8 GeoTransform(C_Vec3_ui8& Im3D, C_Vec_s& name);

//layout to call GeoTransform
Vec3_ui8 CallGeoTransform();

Vec2_ui8 SpatialWarping2D(C_Vec2_ui8& Im2D, C_Vec2_d& coef);
Vec2_ui8 SpatialWarping2D1(C_Vec2_ui8& Im2D, C_Vec2_d& coef);

//opt = 0: mapping a2b
//opt = 1: mapping b2a
Vec3_ui8 CallSpatialWarping(C_Vec3_ui8& Im3D, C_Vec2_d& coef, C_i opt);

//Distortion correction with k1=-0.3536, k2=0.1730
//fx=fy=600
//cx=image heght/2
//cx=image width/2
//using Newton's method
Vec3_ui8 LensDistortionCorrect(C_Vec3_ui8& Im3D);

//using inverse mapping
Vec3_ui8 LensDistortionCorrect1(C_Vec3_ui8& Im3D);

#endif /* GeometricModification_hpp */
