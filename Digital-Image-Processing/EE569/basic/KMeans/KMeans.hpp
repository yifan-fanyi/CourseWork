//
//  KMeans.hpp
//  EE569
//
//  Created by Alex on 2019/3/4.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef KMeans_hpp
#define KMeans_hpp

#include "Basic.hpp"

//Kmeans initialization
//using maximum distance
Vec2_d KmeansInitMaxD(C_Vec2_d& data, C_i& N, C_i& dim);
Vec2_d KmeansInitUni(C_Vec2_d& data, C_i& N);
//clustering
void KmeansCluster(C_Vec2_d& data, Vec2_d& center, Vec1_i& label, C_i& dim, bool& modified);

//if converge then return
//input data[k][i] and N number of class
//k: number of data
//i: number of feature
//dim: number of first feature used
//init 0:uni, 1: max dist
Vec1_i Kmeans(C_Vec2_d& data, C_i& N, C_i& dim, C_i& init);

#endif /* KMeans_hpp */
