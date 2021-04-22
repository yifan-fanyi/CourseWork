//
//  TextureAnalysis.hpp
//  EE569
//
//  Created by Alex on 2019/3/4.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef TextureAnalysis_hpp
#define TextureAnalysis_hpp

//sigma for gaussian
#define HW4Sigma 50.0
//multi layer, must can be devided by both height and width
#define downsample_win 3
//radius of voting area
#define mergelabel_win 13
#define Search_win 21
//average window size
#define Avg_win 3
//way of sub local mean, 0 abs sub, -1 normal sub
#define Sub_mean -1
//number of iteration
#define NumIterTS 20

#include "HW4Basic.hpp"

//texture 1-12 get average feature
//dim 0: different image
//dim 1: Textures
Vec2_d GetAllTexture();

//using KMeans to clustering textures
Vec2_i TextureLabel(C_i& init);

//draw normalized texture
void DrawSortTextureMap(C_Vec2_d& texture);

//average feature in a window
Vec2_d GetTextureWin(C_Vec3_ui8& Im3D, C_i& window);

//average feature in a window + down sampling
Vec2_d GetTextureWin1(C_Vec3_ui8& Im3D, C_i& window);

//segmentation both can use label re-arrangement, multi-scale
//separate texture
Vec2_i TextureSeparate(C_Vec3_ui8& Im3D, C_i& window, C_i& init);
//iterative method
Vec2_i TextureSeparate1(C_Vec3_ui8& Im3D, C_i& window, C_i& init);

//draw separated image no down sample
void DrawSegImg(C_Vec3_ui8& Im3D, C_Vec1_i& label, C_i& window);

//for using down sample
void DrawSegImg1(C_Vec3_ui8& Im3D, C_Vec1_i& label, C_i& window);

//uniform weight
int ChooseMajority(C_Vec2_ui8& ImA, C_Vec2_ui8& ImB, C_i& i, C_i& j, C_i& r);

//weighted by Gaussian
int ChooseMajorityGaussian(C_Vec2_ui8& ImA, C_Vec2_ui8& ImB, C_i& i, C_i& j, C_i& r);

//compute new mean based on previous label
Vec2_d ComputeNewMean(C_Vec2_d& fea, C_Vec1_i& label, C_i& win, C_i& H, C_i& W);

Vec1_i LargestKCluster(Vec1_i& label, C_i& oK, C_i& K);

#endif /* TextureAnalysis_hpp */

