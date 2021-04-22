//
//  FeatureExtractor.hpp
//  EE569
//
//  Created by Alex on 2019/3/6.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef FeatureExtractor_hpp
#define FeatureExtractor_hpp

#include "HW4Basic.hpp"

//select good match pairs
//distance less than ndist*minDistance
void SelectGoodMatch(Mat& descriptors_1, Mat& descriptors_2, vector<DMatch>& matches, C_d& ndist);

vector<KeyPoint> SelectKeyPointOct(vector<KeyPoint> KPs, C_i& Octave);

//match two images using SIFT
void useSIFT(string nImA, string nImB, double ndist);

//match two images using ORB
void useORB(string nImA, string nImB, double ndist);

//match two images using SURF
void useSURF(string nImA, string nImB, double ndist);

void DetectSIFT(vector<vector<KeyPoint>>& KPs, vector<Mat>& DPs);

void KMeansOpencV(Mat& data, Mat& label, Mat& center, C_i& K);

void useBagOfWords();
#endif /* FeatureExtractor_hpp */
