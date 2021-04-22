//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  MorphologicalProcessing.hpp
//  EE569
//
//  Created by Alex on 2019/2/12.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef MorphologicalProcessing_hpp
#define MorphologicalProcessing_hpp

#include "HW3Basic.hpp"

//
Vec2_ui8 Bridging(C_Vec2_ui8& Im2D);

/***hit template is comverted to a binary tree***/
/***all methods below using binarytree search***/
/***iter is used, stop is judged by wether changed input from last iter or max_iter=2000***/
Vec2_ui8 Morphological2D(C_Vec2_ui8& Im2D, BinaryDict* Cond, BinaryDict* UnCond, bool& modify, C_i& Opt);

//Opt = 0: Shrinking
//Opt = 1: Thinning
//Iot = 2: Skeletonizing
Vec3_ui8 MorphologicalProcessing(C_Vec3_ui8& Im3D, C_i& Opt, C_i& maxiter);

//depth first search
//search 0 and save 0's location
int DFS(Vec2_ui8& mat2d, C_i& i, C_i& j, Vec2_i& position);

//search connected pixels(value=0) using DFS
//save location if number of connected pixel less than threshold
//used in rice.raw
//flag = 1 save location if number of connected pixel larger than threshold
//flag = 0 save location if number of connected pixel smaller than threshold
Vec3_i SearchEdges(C_Vec2_ui8& mat2d, C_i& threshold, C_b& flag);

//get edge using 000000000 mask
Vec2_ui8 MaskSearch(C_Vec2_ui8& Im2D, BinaryDict* head);

//Defect detector
Vec3_ui8 DefectDetect(C_Vec3_ui8& Im3D);

//remove defects
Vec3_ui8 ClearDefect(C_Vec3_ui8& Im3D, C_Vec3_i& loc);

//draw defects in color
void DrawDefect(C_Vec3_ui8& Im3D, C_Vec3_i& loc);

//based on background are the majority
//background varies litte
Vec2_ui8 RemoveBackGround(C_Vec2_ui8& Im2D);

//remove noise based on connectity
Vec2_ui8 RemoveNoise(C_Vec2_ui8& Im2D, C_i& threshold);

//output a binarized rice image
Vec3_ui8 RiceProcess1(C_Vec3_ui8& Im3D);

//using DFS count rice number
int RiceCount(C_Vec3_ui8& Im3D);

//based on shape of rice is convex
void FillHole(Vec3_ui8& Im3D, Vec3_i& loc);

//compute geometric center
Vec2_d GetRiceCenter(C_Vec3_i& loc);

//classify rice using KMeans
//based on location
void RiceSize(C_Vec3_ui8& Im3D, C_i& Nclass);

//for drawing
void DrawBar(Vec3_ui8& Im3D, C_i& Nclass);
void DrawClassifiedRice(C_Vec2_ui8& Im2D, C_Vec2_d& cloc, C_Vec1_i& labels, C_i& Nclass);
void DrawSortedRice(Vec3_ui8& Im3D, C_Vec2_d& cloc, C_Vec1_i& labels, map<int, int> mpp, C_i& Nclass);
void DrawDFS(Vec2_ui8& mat2d, C_i& i, C_i& j, C_i& labels, C_i& Nclass);

#endif /* MorphologicalProcessing_hpp */
