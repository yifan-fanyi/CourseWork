//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  main.cpp
//  EE569
//
//  Created by Alex on 2019/1/7.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "HW1Basic.hpp"
#include "HW2Basic.hpp"
#include "HW3Basic.hpp"
#include "HW4Basic.hpp"

using namespace std;

int main(int argc, char *argv[]){
    double StartTime=clock(),EndTime;
    
    //TextureLabel(0);
    Vec3_ui8 Im3D = ReadRaw("deer.raw", 1, 691 , 550);
  //  TextureSeparate(Im3D, 15 , 0);
  //  useBagOfWords();
  //  useSIFT("river1", "river23", 2);
    //MorphologicalProcessing(Im3D, 0, 2000);
    
    DefectDetect(Im3D);
    EndTime = clock();
    cout<<"The during time is: "<< (EndTime - StartTime) / 1000000 <<" seconds\n"<<endl;
    return 0;
}




