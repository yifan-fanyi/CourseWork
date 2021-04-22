//
//  HilbertCurve.cpp
//  EE569
//
//  Created by Alex on 2019/2/1.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "HilbertCurve.hpp"

int deinterleave(int x){
    x = x & 0x55555555;
    x = (x | (x >> 1)) & 0x33333333;
    x = (x | (x >> 2)) & 0x0F0F0F0F;
    x = (x | (x >> 4)) & 0x00FF00FF;
    x = (x | (x >> 8)) & 0x0000FFFF;
    return x;
}

int interleave(int x){
    x = (x | (x << 8)) & 0x00FF00FF;
    x = (x | (x << 4)) & 0x0F0F0F0F;
    x = (x | (x << 2)) & 0x33333333;
    x = (x | (x << 1)) & 0x55555555;
    return x;
}

int prefixScan(int x){
    x = (x >> 8) ^ x;
    x = (x >> 4) ^ x;
    x = (x >> 2) ^ x;
    x = (x >> 1) ^ x;
    return x;
}

int descan(int x){
    return x ^ (x >> 1);
}

void hilbertIndexToXY(int n, int i, int& x, int& y){
    i = i << (32 - 2 * n);
    int i0 = deinterleave(i);
    int i1 = deinterleave(i >> 1);
    int t0 = (i0 | i1) ^ 0xFFFF;
    int t1 = i0 & i1;
    int prefixT0 = prefixScan(t0);
    int prefixT1 = prefixScan(t1);
    int a = (((i0 ^ 0xFFFF) & prefixT1) | (i0 & prefixT0));
    x = (a ^ i1) >> (16 - n);
    y = (a ^ i0 ^ i1) >> (16 - n);
}

