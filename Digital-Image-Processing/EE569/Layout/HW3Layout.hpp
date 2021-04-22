//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW3Layout.hpp
//  EE569
//
//  Created by Alex on 2019/2/4.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HW3Layout_hpp
#define HW3Layout_hpp

#include "HW3Basic.hpp"

void HW3printHelp(){
    cout<<"Geometric Modification:"<<endl;
    cout<<"    Geometric Transformation:"<<endl;
    cout<<"        GeoT, <output>.raw"<<endl;
    cout<<"    Spatial Warping:"<<endl;
    cout<<"        SpatW, <input>.raw, Color, Height, Width, Operation, <output>.raw"<<endl;
    cout<<"    Lens Distortion Correction:"<<endl;
    cout<<"        LensDC, <input>.raw, Color, Height, Width, <output>.raw"<<endl;
    cout<<endl;
    
    cout<<"Morphological Processing:"<<endl;
    cout<<"    Basic Morphological Process:"<<endl;
    cout<<"        BMP, <input>.raw, Color, Height, Width, Operation, MaxIter, <output>.raw"<<endl;
    cout<<"    Defect Detection and Correction:"<<endl;
    cout<<"        Defect, <input>.raw, Color, Height, Width, <output>.raw"<<endl;
    cout<<"    Object Analysis:"<<endl;
    cout<<"        RiceCount, <input>.raw, Color, Height, Width"<<endl;
    cout<<"        RiceSize, <input>.raw, Color, Height, Width, Nclass"<<endl;
    cout<<endl;
    
    cout<<"Quit: Q"<<endl;
    cout<<"Help: H"<<endl;
    cout<<"Return: R"<<endl;
}


map<string, int> HW3funList(){
    map<string, int> mp;
    mp["Q"] = 0;
    mp["H"] = 1;
    mp["R"] = 2;
    
    mp["GeoT"] = 10;
    mp["SpatW"] = 11;
    mp["LensDC"] = 12;
    
    mp["BMP"] = 20;
    mp["Defect"] = 21;
    mp["RiceCount"] = 22;
    mp["RiceSize"] = 23;

    return mp;
}

void HW3run(){
    HW3printHelp();
    map<string, int> mp = HW3funList();
    while(1){
        string str;
        getline(cin, str);
        vector<string> s = separate(str);
        if(mp[s[0]] == 0){exit(1);}
        else if(mp[s[0]] == 1){HW3printHelp();}
        else if(mp[s[0]] == 2){break;}
        else if(mp[s[0]] == 10){
            Vec3_ui8 tmp =  CallGeoTransform();
            WriteRaw(str2pchar(outdata+s[1]), tmp);
        }
        else if(mp[s[0]] == 11){
            Vec1_d coef;
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = CallSpatialWarping(tmp, {}, str2int(s[5]));
            WriteRaw(str2pchar(outdata+s[6]), tmp);
        }
        else if(mp[s[0]] == 12){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = LensDistortionCorrect(tmp);
            WriteRaw(str2pchar(outdata+s[5]), tmp);
        }
        else if(mp[s[0]] == 20){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = MorphologicalProcessing(tmp, str2int(s[5]), str2int(s[6]));
            WriteRaw(str2pchar(outdata+s[7]), tmp);
        }
        else if(mp[s[0]] == 21){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = DefectDetect(tmp);
            WriteRaw(str2pchar(outdata+s[5]), tmp);
        }
        else if(mp[s[0]] == 22){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            RiceCount(tmp);
        }
        else if(mp[s[0]] == 23){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            RiceSize(tmp, str2int(s[5]));
        }
        else{
            cout<<"No such function!"<<endl;
            continue;
        }
    }
}

#endif /* HW3Layout_hpp */
