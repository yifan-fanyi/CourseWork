//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW2Basic.hpp
//  EE569
//
//  Created by Alex on 2019/1/25.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HW2Basic_hpp
#define HW2Basic_hpp

#include "Basic.hpp"

#include "EdgeDetect.hpp"
#include "HalfToning.hpp"
#include "HW2Fancy.hpp"


//gen Index Matrix for Dithering Matrix
//index: power of 2
Vec2_i genIndexMatrix(C_i& order, C_b& normalize, C_i& noise);

//Raster parsing
//threshold: error larger than th or smaller than -threshold will be restricted
//threshold < 0 no restrict
Vec2_ui8 RPSweep2DRefP(C_Vec2_ui8& Im2D, Vec2_d& kernel, C_i& threshold, C_b& normalize);

//Serpentine parsing
//threshold: error larger than th or smaller than -threshold will be restricted
//threshold < 0 no restrict
Vec2_ui8 SPSweep2DRefP(C_Vec2_ui8& Im2D, Vec2_d& kernel, C_i& threshold, C_b& normalize);

//Serpentine parsing specialized for MBVQ-based Error Diffusion
//threshold: error larger than th or smaller than -threshold will be restricted
//threshold < 0 no restrict
Vec3_ui8 SPSweep3DRefPMBVQ(C_Vec3_ui8& Im3D, Vec2_d& kernel, C_i& threshold, C_b& normalize);

//deep layer of ErrorDiffusion
Vec2_ui8 ErrDiffusion2D(C_Vec2_ui8& Im2D, C_i& version, C_i& spread, C_i& threshold);

//step 1 in MBVQ
string MBVQ(C_i& R, C_i& G, C_i& B);

map<char, Vec1_i> BuildCube();

//step 2 and 3 in MBVQ
//using L1 norm
Vec1_i ClosetVertex(C_i& R, C_i& G, C_i& B);

//using decision tree provided
Vec1_i ClosetVertexDT(C_i& R, C_i& G, C_i& B);

#endif /* HW2Basic_hpp */
