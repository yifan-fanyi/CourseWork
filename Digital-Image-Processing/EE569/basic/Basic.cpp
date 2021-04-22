//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  Basic.cpp
//  EE569
//
//  Created by Alex on 2019/1/7.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "Basic.hpp"

Vec3_ui8 ReadRaw(C_c* Path, C_i& K, C_i& H, C_i& W){
    FILE* pFile;
    Vec3_ui8 ImageData init3_ui8(K, H, W, 0);
    pFile = fopen(Path, "rb");
    if(!pFile) {
        cout<<"ReadRaw: Cannot open file: "<<Path<<endl;
        exit(1);
    }
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            for(int k(0); k < K; ++k){
                uint8_t pixVal;
                fread(&pixVal, sizeof(uint8_t), 1, pFile);
                ImageData[k][i][j] = pixVal;
            }
        }
    }
    cout<<"ReadRaw: read "<<Path<<" successfully"<<endl;
    fclose(pFile);
    return ImageData;
}

void WriteRaw(C_c* Path, C_Vec3_ui8& Img3D){
    FILE* pFile;
    if(!(pFile=fopen(Path,"wb"))) {
        cout<<"WriteRaw: Cannot open file: "<<Path<<endl;
        exit(1);
    }
    for(int i(0); i < Dim2(Img3D); ++i){
        for(int j(0); j < Dim3(Img3D); ++j){
            for(int k(0); k < Dim1(Img3D); ++k){
                uint8_t pixVal = Img3D[k][i][j];
                fwrite(&pixVal, sizeof(uint8_t), 1, pFile);
            }
        }
    }
    cout<<"WriteRaw: write "<<Path<<" successfully"<<endl;
    fclose(pFile);
    return;
}

void WriteRaw(C_c* Path, C_Vec2_ui8& Img2D){
    WriteRaw(Path, Vec2toVec3(Img2D));
}

void WriteRaw(C_c* Path, C_Vec2_i& Img2D){
    WriteRaw(Path, Vec2toVec3(toUint(Img2D)));
}

void WriteRaw(C_c* Path, C_Vec2_d& Img2D){
    WriteRaw(Path, Vec2toVec3(toUint(Img2D)));
}

void WriteRaw(C_c* Path, C_Vec3_i& Img3D){
    WriteRaw(Path, toUint(Img3D));
}

void WriteRaw(C_c* Path, C_Vec3_d& Img3D){
    WriteRaw(Path, toUint(Img3D));
}

Vec1_s separate(C_s& s){
    Vec1_s res;
    string tmp;
    for(int i = 0; i < Dim1(s); ++i){
        if(s[i] == ' '){
            res.push_back(tmp);
            tmp.clear();
        }
        else{
            tmp += s[i];
        }
    }
    res.push_back(tmp);
    return res;
}

Vec3_ui8 Mat2Raw(C_c* in_Path){
    cv::Mat Img3D;
    Img3D = cv::imread(in_Path, cv::IMREAD_COLOR);
    Vec3_ui8 res init3_ui8(Img3D.channels(), Img3D.rows, Img3D.cols, 0);
    for(int k(0); k < Img3D.channels(); ++k){
        for(int i(0); i < Img3D.rows; ++i){
            for(int j(0); j < Img3D.cols; ++j){
                res[k][i][j] = toUint(Img3D.at<cv::Vec3b>(i, j)[2-k]);
            }
        }
    }
    return res;
}

cv::Mat Raw2Mat(C_Vec3_ui8& Img3D, C_c* out_Path){
    int K(Dim1(Img3D));
    int H(Dim2(Img3D));
    int W(Dim3(Img3D));
    vector<cv::Point3f> t;
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            float t1, t2, t3;
            if(K == 3){
                t1 = Img3D[2][i][j];
                t2 = Img3D[1][i][j];
                t3 = Img3D[0][i][j];
            }
            else{
                t1 = Img3D[0][i][j];
                t2 = Img3D[0][i][j];
                t3 = Img3D[0][i][j];
            }
            t.push_back({t1, t2, t3});
        }
    }
    cv::Mat res{t};
    res = res.reshape(3, H);
    cv::imwrite(out_Path, res);
    cout<<"Raw2Mat: write "<<out_Path<<" successfully"<<endl;
    return res;
}

cv::Mat Raw2Mat(C_Vec3_i& Img3D, C_c* out_Path){
    return Raw2Mat(toUint(Img3D), out_Path);
}

cv::Mat Raw2Mat(C_Vec3_d& Img3D, C_c* out_Path){
    return Raw2Mat(toUint(Img3D), out_Path);
}

cv::Mat Raw2Mat(C_Vec2_ui8& Img2D, C_c* out_Path){
    return Raw2Mat(Vec2toVec3(Img2D), out_Path);
}

cv::Mat Raw2Mat(C_Vec2_i& Img2D, C_c* out_Path){
    return Raw2Mat(Vec2toVec3(Img2D), out_Path);
}

cv::Mat Raw2Mat(C_Vec2_d& Img2D, C_c* out_Path){
    return Raw2Mat(Vec2toVec3(Img2D), out_Path);
}

cv::Mat Raw2Mat(C_c* in_Path, C_c* out_Path, C_i& K, C_i& H, C_i& W){
    Vec3_ui8 Img3D = ReadRaw(in_Path, K, H, W);
    return Raw2Mat(Img3D, out_Path);
}

