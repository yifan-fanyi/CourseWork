//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW2Layout.hpp
//  EE569
//
//  Created by Alex on 2019/1/25.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HW2Layout_hpp
#define HW2Layout_hpp


#include "HW2Basic.hpp"

void HW2printHelp(){
    cout<<"Edge Detection:"<<endl;
    cout<<"    Sobel:"<<endl;
    cout<<"        Sobel, <input>.raw, Color, Height, Width, rate, <output>.raw"<<endl;
    cout<<"    Canny (use OpenCv):"<<endl;
    cout<<"        Canny, <input>.jpg, lowThreshold, highThreshold, kernel_size, <output>.jpg"<<endl;
    cout<<endl;
    
    cout<<"Digital Half Toning:"<<endl;
    cout<<"    Dithering:"<<endl;
    cout<<"        Dith, <input>.raw, Color, Height, Width, <output>.raw"<<endl;
    cout<<"    Dithering Matrix:"<<endl;
    cout<<"        DithM, <input>.raw, Color, Height, Width, Index, th, <output>.raw"<<endl;
    cout<<"    Error Diffusion:"<<endl;
    cout<<"        ErrDif, <input>.raw, Color, Height, Width, CoreVersion, SpreadMethod, TreatAsColor(0/1), Threshold <output>.raw"<<endl;
    cout<<"    MBVQ-based Error Diffusion (Color only):"<<endl;
    cout<<"        MBVQ, <input>.raw, Height, Width, CoreVersion, Threshold, <output>.raw"<<endl;
    cout<<endl;
    
    cout<<"Fancy:"<<endl;
    cout<<"    Messing (elimate artficts):"<<endl;
    cout<<"        **method: 0: DitheringMatrix; 1: Error Diffusion; 2: MBVQ based**"<<endl;
    cout<<"        Mess, <input>.raw, Color, Height, Width, Window, Method, <output>.raw"<<endl;
    cout<<endl;
    
    cout<<"Quit: Q"<<endl;
    cout<<"Help: H"<<endl;
    cout<<"Return: R"<<endl;
}

map<string, int> HW2funList(){
    map<string, int> mp;
    mp["Q"] = 0;
    mp["H"] = 1;
    mp["R"] = 2;
    mp["Sobel"] = 10;
    mp["Canny"] = 11;
    mp["Dith"] = 20;
    mp["DithM"] = 21;
    mp["ErrDif"] = 22;
    mp["MBVQ"] = 23;
    mp["Mess"] = 30;
    return mp;
}

void HW2run(){
    HW2printHelp();
    map<string, int> mp = HW2funList();
    while(1){
        string str;
        getline(cin, str);
        vector<string> s = separate(str);
        if(mp[s[0]] == 0){exit(1);}
        else if(mp[s[0]] == 1){HW2printHelp();}
        else if(mp[s[0]] == 2){break;}
        else if(mp[s[0]] == 10){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            Vec3_ui8 tmp1 = SobelED(tmp, str2double(s[5]));
            WriteRaw(str2pchar(outdata+s[6]), tmp1);
        }
        else if(mp[s[0]] == 11){
            testCanny(str2pchar(indata+s[1]), str2pchar(outdata+s[5]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
        }
        else if(mp[s[0]] == 20){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = Dithering(tmp);
            WriteRaw(str2pchar(outdata+s[5]), tmp);
        }
        else if(mp[s[0]] == 21){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = DitheringMatrix(tmp, str2int(s[5]), str2int(s[6]));
            WriteRaw(str2pchar(outdata+s[7]), tmp);
        }
        else if(mp[s[0]] == 22){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = ErrorDiffusion(tmp, str2int(s[5]), str2int(s[6]),str2int(s[7]), str2int(s[8]));
            WriteRaw(str2pchar(outdata+s[9]), tmp);
        }
        else if(mp[s[0]] == 23){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = MBVQErrDiff(tmp, str2int(s[5]), str2int(s[6]));
            WriteRaw(str2pchar(outdata+s[7]), tmp);
        }
        else if(mp[s[0]] == 30){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            tmp = HW2Fancy1(tmp, str2int(s[5]), str2int(s[6]));;
            WriteRaw(str2pchar(outdata+s[7]), tmp);
        }
        else{
            cout<<"No such function!"<<endl;
            continue;
        }
    }
}

#endif /* HW2Layout_hpp */
