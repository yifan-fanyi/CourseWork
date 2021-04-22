//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  core.hpp
//  EE569
//
//  Created by Alex on 2019/2/7.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef core_hpp
#define core_hpp

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <map>
#include <queue>
#include <cmath>
#include <string>
#include <sstream>
#include <time.h>
#include <random>
#include <stack>
#include <omp.h>
#include <fstream>
/*
#include <opencv2/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/types_c.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
*/
using namespace std;

//data location
#define indata "data/"
#define outdata "data/"

typedef vector<vector<vector<uint8_t>>> Vec3_ui8;
typedef vector<vector<uint8_t>> Vec2_ui8;
typedef vector<uint8_t> Vec1_ui8;
typedef vector<vector<vector<int>>> Vec3_i;
typedef vector<vector<int>> Vec2_i;
typedef vector<int> Vec1_i;
typedef vector<vector<vector<double>>> Vec3_d;
typedef vector<vector<double>> Vec2_d;
typedef vector<double> Vec1_d;
typedef vector<string> Vec1_s;
typedef vector<vector<bool>> Vec2_b;
typedef vector<bool> Vec1_b;

typedef const uint8_t C_ui8;
typedef const int C_i;
typedef const double C_d;
typedef const bool C_b;
typedef const string C_s;
typedef const char C_c;
typedef const Vec3_ui8 C_Vec3_ui8;
typedef const Vec2_ui8 C_Vec2_ui8;
typedef const Vec1_ui8 C_Vec1_ui8;
typedef const Vec3_i C_Vec3_i;
typedef const Vec2_i C_Vec2_i;
typedef const Vec1_i C_Vec1_i;
typedef const Vec3_d C_Vec3_d;
typedef const Vec2_d C_Vec2_d;
typedef const Vec1_d C_Vec1_d;
typedef const Vec1_s C_Vec_s;
typedef const Vec2_b C_Vec2_b;
typedef const Vec1_b C_Vec1_b;

#define Vec1_T vector<T>
#define Vec2_T vector<vector<T>>
#define Vec3_T vector<vector<vector<T>>>
#define init3_T(K, H, W, a) (K, Vec2_T(H, Vec1_T(W, a)))
#define init2_T(H, W, a) (H, Vec1_T(W, a))

#define init3_ui8(K, H, W, a) (K, Vec2_ui8(H, Vec1_ui8(W, a)))
#define init2_ui8(H, W, a) (H, Vec1_ui8(W, a))
#define init3_i(K, H, W, a) (K, Vec2_i(H, Vec1_i(W, a)))
#define init2_i(H, W, a) (H, Vec1_i(W, a))
#define init3_d(K, H, W, a) (K, Vec2_d(H, Vec1_d(W, a)))
#define init2_d(H, W, a) (H, Vec1_d(W, a))
#define init2_b(H, W, a) (H, Vec1_b(W, a))

#define Dim1(mat) (int)mat.size()
#define Dim2(mat) (int)mat[0].size()
#define Dim3(mat) (int)mat[0][0].size()

#define Int_RV static default_random_engine IntRand(time(NULL))
#define RV_Uni static uniform_int_distribution<int>

#define Pi 3.14159265

template <class T, class T1>
inline T Max(T a, T1 b){
    T res = a > b ? a : b;
    return res;
}

template <class T, class T1>
inline T Min(T a, T1 b){
    T res = a < b ? a : b;
    return res;
}

template <class T>
inline T Sq(T x){
    return x*x;
}

//string to int
inline int str2int(C_s& s){
    istringstream is(s);
    int res;
    is>>res;
    return res;
}

//string to double
inline double str2double(C_s& s){
    istringstream is(s);
    double res;
    is>>res;
    return res;
}

//string to char*
inline C_c* str2pchar(C_s& s){
    C_c *p = s.c_str();
    return p;
}

//int/double to uint8_t
inline uint8_t toUint(C_d& t){
    return Max(0, Min(t, 255));
}

inline uint8_t toUint(C_i& t){
    return Max(0, Min(t, 255));
}

inline Vec2_ui8 toUint(C_Vec2_i& Im2D){
    Vec2_ui8 res init2_ui8(Dim1(Im2D), Dim2(Im2D), 0);
    for(int i(0); i < Dim1(Im2D); ++i){
        for(int j(0); j < Dim2(Im2D); ++j){
            res[i][j] = toUint(Im2D[i][j]);
        }
    }
    return res;
}

inline Vec2_ui8 toUint(C_Vec2_d& Im2D){
    Vec2_ui8 res init2_ui8(Dim1(Im2D), Dim2(Im2D), 0);
    for(int i(0); i < Dim1(Im2D); ++i){
        for(int j(0); j < Dim2(Im2D); ++j){
            res[i][j] = toUint(Im2D[i][j]);
        }
    }
    return res;
}

inline Vec3_ui8 toUint(C_Vec3_i& Im3D){
    Vec3_ui8 res;
    for(int k(0); k < Dim1(Im3D); ++k){
        res.push_back(toUint(Im3D[k]));
    }
    return res;
}

inline Vec3_ui8 toUint(C_Vec3_d& Im3D){
    Vec3_ui8 res;
    for(int k(0); k < Dim1(Im3D); ++k){
        res.push_back(toUint(Im3D[k]));
    }
    return res;
}

inline Vec2_d toDouble(C_Vec2_ui8& Im2D){
    Vec2_d res init2_d(Dim1(Im2D), Dim2(Im2D), 0);
    for(int i(0); i < Dim1(Im2D); ++i){
        for(int j(0); j < Dim2(Im2D); ++j){
            res[i][j] = (double)Im2D[i][j];
        }
    }
    return res;
}

inline Vec3_ui8 Vec2toVec3(C_Vec2_ui8& mat2d){
    Vec3_ui8 t;
    t.push_back(mat2d);
    return t;
}

inline Vec3_i Vec2toVec3(C_Vec2_i& mat2d){
    Vec3_i t;
    t.push_back(mat2d);
    return t;
}

inline Vec3_d Vec2toVec3(C_Vec2_d& mat2d){
    Vec3_d t;
    t.push_back(mat2d);
    return t;
}

#endif /* core_hpp */
