//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW1Layout.hpp
//  EE569
//
//  Created by Alex on 2019/1/25.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HW1Layout_hpp
#define HW1Layout_hpp

#include "HW1Basic.hpp"

void HW1printHelp(){
    cout<<"Image Demosaicing:"<<endl;
    cout<<"    BiDemosaicing:"<<endl;
    cout<<"        BiD, <input>.raw, Color, Height, Width, <output>.raw"<<endl;
    cout<<"    MHCDemosaicing:"<<endl;
    cout<<"        MHCD, <input>.raw, Color, Height, Width, <output>.raw"<<endl;
    
    cout<<"Histogram Equalization:"<<endl;
    cout<<"    saveHistogram:"<<endl;
    cout<<"        saveHist, <input>.raw, Color, Height, Width, <output>.raw"<<endl;
    cout<<"    HistEqMethodA:"<<endl;
    cout<<"        HEqMA, <input>.raw, Color, Height, Width, <output>.raw"<<endl;
    cout<<"    HistEqMethodB:"<<endl;
    cout<<"        HEqMB, <input>.raw, Color, Height, Width, <output>.raw"<<endl;
    cout<<endl;
    cout<<"Image Denoising:"<<endl;
    cout<<"    getNoise:"<<endl;
    cout<<"        getN, <input>.raw, <noise input>.raw, Color, Height, Width, <output>.raw"<<endl;
    cout<<"    UniformFilter:"<<endl;
    cout<<"        UniF, <input>.raw, Color, Height, Width, CoreSize, <output>.raw"<<endl;
    cout<<"    Gaussian Filter:"<<endl;
    cout<<"        GauF, <input>.raw, Color, Height, Width, CoreSize, Sigma, <output>.raw"<<endl;
    cout<<"    BilateralFilter:"<<endl;
    cout<<"        BilF, <input>.raw, Color, Height, Width, CoreSize, SigmaD, SigmaR, <output>.raw"<<endl;
    cout<<"    NLMeansFilter:"<<endl;
    cout<<"        NLMF, <input>.raw, Color, Height, Width, Sigma, SearchW, CompareW, <output>.raw"<<endl;
    cout<<"    NLMeansFilterG:"<<endl;
    cout<<"        NLMFG, <input>.raw, Color, Height, Width, Sigma, SigmaG, SearchW, CompareW, <output>.raw"<<endl;
    cout<<"    ShotNoise:"<<endl;
    cout<<"        ShotN, <input>.raw, Color, Height, Width, CoreSize, Sigma, <output>.raw"<<endl;
    
    cout<<endl;
    cout<<"Evaluation:"<<endl;
    cout<<"    calSNR:"<<endl;
    cout<<"        SNR, <reference input>.raw, <input>.raw, Color, Height, Width"<<endl;
    cout<<"    calPSNR:"<<endl;
    cout<<"        PSNR, <reference input>.raw, <input>.raw, Color, Height, Width"<<endl;
    cout<<"    calSSIM:"<<endl;
    cout<<"        SSIM, <reference input>.raw, <input>.raw, Color, Height, Width"<<endl;
    cout<<endl;
    cout<<"Demo:"<<endl;
    cout<<"    Demo on pipeline filter on rose_color_noise.raw:"<<endl;
    cout<<"        Demo1"<<endl;
    cout<<endl;
    
    cout<<"Quit: Q"<<endl;
    cout<<"Help: H"<<endl;
    cout<<"Return: R"<<endl;
}

map<string, int> HW1funList(){
    map<string, int> mp;
    mp["Q"] = 0;
    mp["H"] = 1;
    mp["R"] = 2;
    
    mp["BiD"] = 10;
    mp["MHCD"] = 11;
    mp["saveHist"] = 12;
    mp["HEqMA"] = 13;
    mp["HEqMB"] = 14;
    
    mp["getN"] = 20;
    mp["UniF"] = 21;
    mp["GauF"] = 22;
    mp["BilF"] = 23;
    mp["NLMF"] = 24;
    mp["NLMFG"] = 25;
    mp["ShotN"] = 26;
    
    mp["Demo1"] = 99;
    mp["SNR"] = 100;
    mp["PSNR"] = 101;
    mp["SSIM"] = 102;
    return mp;
}

void HW1run(){
    HW1printHelp();
    map<string, int> mp = HW1funList();
    while(1){
        string str;
        getline(cin, str);
        vector<string> s = separate(str);
        
        if(mp[s[0]] == 0){exit(1);}
        else if(mp[s[0]] == 1){HW1printHelp();}
        if(mp[s[0]] == 2){break;}

        else if(mp[s[0]] == 10){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), 1, str2int(s[2]), str2int(s[3]));
            Vec3_ui8 tmp1 = BiDemosaicing(tmp);
            WriteRaw(str2pchar(outdata+s[4]), tmp1);
        }
        else if(mp[s[0]] == 11){
        Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), 1, str2int(s[2]), str2int(s[3]));
        tmp = MHCDemosaicing(tmp);
        WriteRaw(str2pchar(outdata+s[4]), tmp);
        }
        else if(mp[s[0]] == 12){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            saveHistogram(tmp, str2pchar(outdata+s[5]));
        }
        else if(mp[s[0]] == 13){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = HistEqMethodA(tmp);
            WriteRaw(str2pchar(outdata+s[5]), tmp);
        }
        else if(mp[s[0]] == 14){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = HistEqMethodB(tmp);
            WriteRaw(str2pchar(outdata+s[5]), tmp);
        }
    
        else if(mp[s[0]] == 20){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[3]), str2int(s[4]), str2int(s[5]));
            Vec3_ui8 tmp1 = ReadRaw(str2pchar(indata+s[2]), str2int(s[3]), str2int(s[4]), str2int(s[5]));
            tmp = getNoise(tmp, tmp1);
            WriteRaw(str2pchar(outdata+s[6]), tmp1);
        }
        else if(mp[s[0]] == 21){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = UniformFilter(tmp, str2int(s[5]));
            WriteRaw(str2pchar(outdata+s[6]), tmp);
        }
        else if(mp[s[0]] == 22){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = GaussianFilter(tmp, str2int(s[5]), str2double(s[6]));
            WriteRaw(str2pchar(outdata+s[7]), tmp);
        }
        else if(mp[s[0]] == 23){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = BilateralFilter(tmp, str2int(s[5]), str2double(s[6]), str2double(s[7]));
            WriteRaw(str2pchar(outdata+s[8]), tmp);
        }
        else if(mp[s[0]] == 24){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = NLMeansFilter(tmp, str2double(s[5]), str2int(s[6]), str2int(s[7]));
            WriteRaw(str2pchar(outdata+s[8]), tmp);
        }
        else if(mp[s[0]] == 25){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = NLMeansFilterG(tmp, str2double(s[5]), str2double(s[6]), str2int(s[7]), str2int(s[8]));
            WriteRaw(str2pchar(outdata+s[9]), tmp);
        }
        else if(mp[s[0]] == 26){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = ShotNoise(tmp, str2int(s[5]), str2double(s[6]));
            WriteRaw(str2pchar(outdata+s[7]), tmp);
        }
    
        else if(mp[s[0]] == 99){
            HW1Fancy1();
        }
        else if(mp[s[0]] == 100){
            calSNR(str2pchar(indata+s[1]), str2pchar(indata+s[2]), str2int(s[3]), str2int(s[4]), str2int(s[5]));
        }
        else if(mp[s[0]] == 101){
            calPSNR(str2pchar(indata+s[1]), str2pchar(indata+s[2]), str2int(s[3]), str2int(s[4]), str2int(s[5]));
        }
        else if(mp[s[0]] == 102){
            calSSIM(str2pchar(indata+s[1]), str2pchar(indata+s[2]), str2int(s[3]), str2int(s[4]), str2int(s[5]));
        }
        else{
            cout<<"No such function!"<<endl;
            continue;
        }
    }
}

#endif /* HW1Layout_hpp */
