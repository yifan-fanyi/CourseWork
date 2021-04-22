//
//  HW4Basic.hpp
//  EE569
//
//  Created by Alex on 2019/3/4.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HW4Basic_hpp
#define HW4Basic_hpp

#include "Basic.hpp"
#include "HW1Basic.hpp"
#include "HW3Basic.hpp"

#include "TextureAnalysis.hpp"
#include "FeatureExtractor.hpp"

//generate laws filter
map<string, Vec2_d> LawsFilter2D();

//extract textures using dot product of 1d textures
//pre processed
map<string, Vec2_d> TextureExtraction(C_Vec2_d& Im2D);

//LE-EL LR-RL ES-SE SS RR LS-SL EE ER-RE SR-RS
Vec3_d GetNeededEnergyMap(map<string, Vec2_d>& texture);

//average textures of whole image
Vec1_d AverageTexture(C_Vec3_d& Im3D);

//pca using OpenCV PCA
Vec2_d myPCA(C_Vec2_d& data, C_i& keepdim);

//normalize texture to (0,1)
void NormalizeTexture(Vec2_d& feature);

//normalize texture by matrix LL
//LL same size as texture[i]
void NormalizeTextureLL(Vec3_d& texture, C_Vec2_d& LL);

#endif /* HW4Basic_hpp */
