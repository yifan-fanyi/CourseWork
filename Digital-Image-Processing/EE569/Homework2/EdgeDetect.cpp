//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  EdgeDetect.cpp
//  EE569
//
//  Created by Alex on 2019/1/21.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "EdgeDetect.hpp"

Vec3_ui8 SobelED(C_Vec3_ui8& Im3D, C_d& keep_rate){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D));
    Vec3_ui8 Im (Im3D);
    if(K == 3){
        Im = RGB2Gray(Im3D);
    }
    Vec3_ui8 res init3_ui8(1, H, W, 255);
    Vec2_d xkernel ({{-0.25, 0, 0.25}, {-0.5, 0, 0.5}, {-0.25, 0, 0.25}});
    Vec2_d ykernel ({{-0.25, -0.5, -0.25}, {0, 0, 0}, {0.25, 0.5, 0.25}});
    //more precise weight
    //Vec2_d xkernel({{-0.25, 0, 0.25}, {-sqrt(0.5), 0, sqrt(0.5)}, {-0.25, 0, 0.25}});
    //Vec2_d ykernel({{-0.25, -sqrt(0.5), -0.25}, {0, 0, 0}, {0.25, sqrt(0.5), 0.25}});
    Vec2_d xdir (Conv2D_Ref(Im[0], xkernel, false));
    Vec2_d ydir (Conv2D_Ref(Im[0], ykernel, false));
    WriteRaw("sobel_xdir_grad.raw", xdir);
    WriteRaw("sobel_ydir_grad.raw", ydir);
    Vec2_d tmp (xdir);
    map<double, queue<Vec1_i>> mp;
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            tmp[i][j] = sqrt(Sq(xdir[i][j]) + Sq(ydir[i][j]));
            mp[tmp[i][j]].push({i, j});
        }
    }
    map<double, queue<Vec1_i>>::reverse_iterator riter = mp.rbegin();
    int N (keep_rate*H*W), ct (0);
    while(riter != mp.rend() && ct <= N){
        res[0][riter->second.front()[0]][riter->second.front()[1]] = 0;
        riter->second.pop();
        ++ct;
        if(riter->second.empty() == true){
            ++riter;
        }
    }
    return res;
}

void testCanny(C_c* in_Path, C_c* out_Path, C_i& low_threshold, C_i& high_threshold, C_i& kernel){
    cv::Mat Im3D, gray, edge, draw, oppedge;
    Im3D = cv::imread(in_Path, cv::IMREAD_COLOR);
    cv::cvtColor(Im3D, gray, CV_BGR2GRAY);
    cv::Canny(gray, edge, low_threshold, high_threshold, kernel);
    edge.convertTo(draw, CV_8U);
    cv::bitwise_not(edge,oppedge);
    cv::imwrite(out_Path, oppedge);
}

