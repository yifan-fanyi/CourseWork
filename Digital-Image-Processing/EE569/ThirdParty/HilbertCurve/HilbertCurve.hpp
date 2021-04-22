//
//  HilbertCurve.hpp
//  EE569
//
//  Created by Alex on 2019/2/1.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HilbertCurve_hpp
#define HilbertCurve_hpp

//connvert index to 2D x y location
//called by HW2Fancy.hpp
//using code from:
//https://github.com/rawrunprotected/hilbert_curves/blob/master/hilbert_curves.cpp

#include <cstdint>

int deinterleave(int x);

int interleave(int x);

int prefixScan(int x);

int descan(int x);

void hilbertIndexToXY(int n, int i, int& x, int& y);
#endif /* HilbertCurve_hpp */
