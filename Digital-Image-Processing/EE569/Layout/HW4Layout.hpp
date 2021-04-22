//
//  HW4Layout.hpp
//  EE569
//
//  Created by Alex on 2019/3/4.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HW4Layout_hpp
#define HW4Layout_hpp

#include "HW4Basic.hpp"

void HW4printHelp(){
    cout<<"Texture Analysis:"<<endl;
    cout<<"    Cluster Texture:"<<endl;
    cout<<"        CT, initial"<<endl;
    cout<<"    Texture Segmentation:"<<endl;
    cout<<"        TSep, <input>.raw, Color, Height, Width, Window, initial, <output>.raw"<<endl;
    cout<<"    Texture Segmentation Fancy:"<<endl;
    cout<<"        TSepF, <input>.raw, Color, Height, Width, Window, initial, <output>.raw"<<endl;
    cout<<endl;
    
    cout<<"Image Feature Extractor:"<<endl;
    cout<<"    SIFT:"<<endl;
    cout<<"        SIFT, name1, name2, dist"<<endl;
    cout<<"    ORB:"<<endl;
    cout<<"        ORB, name1, name2, dist"<<endl;
    cout<<"    SURF:"<<endl;
    cout<<"        SURF, name1, name2, dist"<<endl;
    cout<<"    Bag of Words:"<<endl;
    cout<<"        BoW"<<endl;
    cout<<endl;
    
    cout<<"Quit: Q"<<endl;
    cout<<"Help: H"<<endl;
    cout<<"Return: R"<<endl;
}


map<string, int> HW4funList(){
    map<string, int> mp;
    mp["Q"] = 0;
    mp["H"] = 1;
    mp["R"] = 2;
    
    mp["CT"] = 10;
    mp["TSep"] = 11;
    mp["TSepF"] = 12;
    
    mp["SIFT"] = 20;
    mp["ORB"] = 21;
    mp["SURF"] = 22;
    mp["BOW"] = 23;
    
    return mp;
}

void HW4run(){
    HW4printHelp();
    map<string, int> mp = HW4funList();
    while(1){
        string str;
        getline(cin, str);
        vector<string> s = separate(str);
        if(mp[s[0]] == 0){exit(1);}
        else if(mp[s[0]] == 1){HW4printHelp();}
        else if(mp[s[0]] == 2){break;}
        else if(mp[s[0]] == 10){
            TextureLabel(str2int(s[1]));
        }
        else if(mp[s[0]] == 11){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            Vec2_i t = TextureSeparate(tmp, str2int(s[5]), str2int(s[6]));
            WriteRaw(str2pchar(outdata+s[7]), t);
        }
        else if(mp[s[0]] == 12){
            Vec3_ui8 tmp = ReadRaw(str2pchar(indata+s[1]), str2int(s[2]), str2int(s[3]), str2int(s[4]));
            Vec2_i t = TextureSeparate1(tmp, str2int(s[5]), str2int(s[6]));
            WriteRaw(str2pchar(outdata+s[7]), t);
        }
        
        else if(mp[s[0]] == 20){
            useSIFT(s[1], s[2], str2double(s[3]));
        }
        else if(mp[s[0]] == 21){
            useORB(s[1], s[2], str2double(s[3]));
        }
        else if(mp[s[0]] == 22){
            useSURF(s[1], s[2], str2double(s[3]));
        }
        else if(mp[s[0]] == 23){
            useBagOfWords();
        }
        else{
            cout<<"No such function!"<<endl;
            continue;
        }
    }
}


#endif /* HW4Layout_hpp */
