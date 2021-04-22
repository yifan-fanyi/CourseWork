//
//  HW2.h
//  CSCI576
//
//  Created by Alex on 2021/2/22.
//

#ifndef HW2_h
#define HW2_h

#include <string>
#include "utli.h"
#include "Image.h"

void RGB2HSV_pixel(const double R, const double G, const double B, double& H, double& S, double& V){
    double tR = R / 255.0, tG = G / 255.0, tB = B / 255.0;
    double cmax  = Max(tR, Max(tG, tB));
    double cmin  = Min(tR, Min(tG, tB));
    double delta = cmax - cmin;
    if (delta < 1e-10){
        H = 0;
    }
    else if (tR >= cmax-1e-10){
        H = 60 * Mod((tG - tB) / delta, 6);
    }
    else if (tG >= cmax-1e-10){
        H = 60 * ((tB - tR) / delta + 2);
    }
    else{
        H = 60 * ((tR - tG) / delta + 4);
    }
    if (H < 0){
        H += 360;
    }
    if (H > 360){
        H -= 360;
    }
    S = cmax < 1e-10 ? 0 : delta / cmax;
    V = cmax;
}

void RGB2HSV(const char* RGB, double* HSV, const int H, const int W){
    for (int i = 0; i < H * W; i++){
        double R = (uint8_t)(RGB[3*i  ]);
        double G = (uint8_t)(RGB[3*i+1]);
        double B = (uint8_t)(RGB[3*i+2]);
        double H, S, V;
        RGB2HSV_pixel(R, G, B, H, S, V);
        HSV[3*i]   = H;
        HSV[3*i+1] = S;
        HSV[3*i+2] = V;
    }
}

void HSV2RGB_pixel(const double H, const double S, const double V, double& R, double& G, double& B){
    double C = V * S;
    double X = C * (1 - Abs(Mod(H / 60, 2) - 1));
    double m = V - C;
    double tR, tG, tB;
    if (H < 60){
        tR = C; tG = X; tB = 0;
    }
    else if (H < 120){
        tR = X; tG = C; tB = 0;
    }
    else if (H < 180){
        tR = 0; tG = C; tB = X;
    }
    else if (H < 240){
        tR = 0; tG = X; tB = C;
    }
    else if (H < 300){
        tR = X; tG = 0; tB = C;
    }
    else{
        tR = C; tG = 0; tB = X;
    }
    R = (tR + m) * 255;
    G = (tG + m) * 255;
    B = (tB + m) * 255;
}

void HSV2RGB(const double* HSV, double* RGB, const int H, const int W){
    for (int i = 0; i < H * W; i++){
        double R, G, B;
        HSV2RGB_pixel(HSV[3*i], HSV[3*i+1], HSV[3*i+2], R, G, B);
        RGB[3*i]   = R;
        RGB[3*i+1] = G;
        RGB[3*i+2] = B;
    }
}

double HSV_Segment_pixel_mask(const double H, const double TH1, const double TH2){
    double mask1 = (H >= TH1) ? 1 : 0;
    double mask2 = (H <= TH2) ? 1 : 0;
    return mask1 * mask2;
}

void HSV_Segment(const double* HSV, double* segHSV, const int H, const int W, const double TH1, const double TH2){
    for (int i = 0; i < H * W; i++){
        double mask   = HSV_Segment_pixel_mask(HSV[3*i], TH1, TH2);
        segHSV[3*i]   = HSV[3*i] * mask;
        segHSV[3*i+1] = HSV[3*i+1] * mask;
        segHSV[3*i+2] = HSV[3*i+2];
    }
}

void HW2_Pipline(const string name, const int H, const int W, const double TH1, const double TH2){
    // Load images
    MyImage IMG;
    IMG.setImagePath(str2pchar(name));
    IMG.setWidth(W);
    IMG.setHeight(H);
    IMG.ReadImage();
    double* HSV = new double[H*W*3];

    // RGB2HSV
    RGB2HSV(IMG.getImageData(), HSV, H, W);

    // segment
    double* segHSV = new double[H*W*3];
    HSV_Segment(HSV, segHSV, H, W, TH1, TH2);
    delete[] HSV;
    
    // HSV2RGB
    double* segRGB = new double[H*W*3];
    HSV2RGB(segHSV, segRGB, H, W);
    delete[] segHSV;
    
    
    long int pos = name.find(".");
    string save = name.substr (0,pos) + "_" + \
                   to_string((int)TH1) + "_" + \
                   to_string((int)TH2) + ".raw";
    save_RGB(segRGB, save, H, W);
}

void run_HW2(int argc, const char * argv[], const int H, const int W){
    if (argc < 4){
        printf("\nERROR! Too less arguments! \n");
        printf("   HW2 Usage: ./main <image> <TH1> <TH2>\n\n");
        exit(1);
    }
    HW2_Pipline(argv[1], H, W, str2double(argv[2]), str2double(argv[3]));
}
#endif /* HW2_h */
