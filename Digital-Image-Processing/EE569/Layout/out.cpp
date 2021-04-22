//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  out.cpp
//  EE569
//
//  Created by Alex on 2019/1/11.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "HW1Layout.hpp"
#include "HW2Layout.hpp"
#include "HW3Layout.hpp"
#include "HW4Layout.hpp"

using namespace std;

void printM(){
    cout<<endl;
    cout<<"EE569 HomeWork by Yifan Wang"<<endl;
    cout<<"    yifanwang0916@outlook.com"<<endl;
    cout<<endl;
    cout<<"Using only one space to separate 2 or more input parmeters!"<<endl;
    cout<<"    Error input would lead to exit!"<<endl;
    cout<<"Homework 1: "<<endl;
    cout<<"    HW1"<<endl;
    cout<<"Homework 2: "<<endl;
    cout<<"    HW2"<<endl;
    cout<<"Homework 3: "<<endl;
    cout<<"    HW3"<<endl;
    cout<<"Homework 4: "<<endl;
    cout<<"    HW4"<<endl;
    cout<<"Quit: Q"<<endl;
    cout<<"Help: H"<<endl;
}

map<string, int> mainfunList(){
    map<string, int> mp;
    mp["Q"] = 0;
    mp["H"] = 1;
    
    mp["HW1"] = 10;
    mp["HW2"] = 11;
    mp["HW3"] = 12;
    mp["HW4"] = 13;
    return mp;
}

int main(){
    printM();
    map<string, int> Mainmp = mainfunList();
    while(1){
        string str;
        getline(cin, str);
        vector<string> s = separate(str);
        if(Mainmp[s[0]] == 0){exit(1);}
        else if(Mainmp[s[0]] == 1){printM();}
        else if(Mainmp[s[0]] == 10){HW1run();}
        else if(Mainmp[s[0]] == 11){HW2run();}
        else if(Mainmp[s[0]] == 12){HW3run();}
        else if(Mainmp[s[0]] == 13){HW4run();}
        else{
            cout<<"No such function!"<<endl;
            continue;
        }
        printM();
    }
    return 1;
}

