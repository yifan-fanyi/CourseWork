//
//  BAC.hpp
//  EE669
//
//  Created by Alex on 2019/9/19.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef BAC_hpp
#define BAC_hpp

#include "HW2_utli.hpp"

const vector<vector<int>> BAC_StateTable = {
    {0, 0x59EB, 1, -1},{1, 0x5522, 1, 1},{2, 0x504F, 1, 1},{3, 0x4B85, 1, 1},
    {4, 0x4639, 1, 1},{5, 0x415E, 1, 1},{6, 0x3C3D, 1, 1},{7, 0x375E, 1, 1},
    {8, 0x32B4, 1, 2},{9, 0x2E17, 1, 1},{10, 0x299A, 1, 2},{11, 0x2516, 1, 1},
    {12, 0x1EDF, 1, 1},{13, 0x1AA9, 1, 1},{14, 0x174E, 1, 1},{15, 0x1424, 1, 1},
    {16, 0x119C, 1, 1},{17, 0x0F6B, 1, 2},{18, 0x0D51, 1, 2},{19, 0x0BB6, 1, 1},
    {20, 0x0A40, 1, 2},{21, 0x0861, 1, 2},{22, 0x0706, 1, 2},{23, 0x05CD, 1, 2},
    {24, 0x04DE, 1, 1},{25, 0x040F, 1, 2},{26, 0x0363, 1, 2},{27, 0x02D4, 1, 2},
    {28, 0x025C, 1, 2},{29, 0x01F8, 1, 2},{30, 0x01A4, 1, 2},{31, 0x0160, 1, 2},
    {32, 0x0125, 1, 2},{33, 0x00F6, 1, 2},{34, 0x00CB, 1, 2},{35, 0x00AB, 1, 1},
    {36, 0x008F, 1, 2},{37, 0x0068, 1, 2},{38, 0x004E, 1, 2},{39, 0x003B, 1, 2},
    {40, 0x002C, 1, 2},{41, 0x001A, 1, 3},{42, 0x000D, 1, 2},{43, 0x0006, 1, 2},
    {44, 0x0003, 1, 2},{45, 0x0001, 0, 1}};

void BAC_Renormalize(unsigned int& A, unsigned int& C, bool& carry, string& out){
    if(carry == true){
        out += "1";
        carry = false;
    }
    while(A < 0x8000){
        A = A<<1;
        if((C & (0x8000)) == 0){
            out += "0";
        }
        else{
            out += "1";
        }
        C = C<<1;
        toInt16(C);
    }
}

void BAC_main(string& in, string& out, unsigned int& A, unsigned int& C, unsigned int& Qe, int& currentState, int& MPS, bool& carry){
    for(int j = 0; j < in.length(); ++j){
        if((int)(in[j]-'0') == MPS){
            A -= Qe;
            if(A < 0x8000){
                if(A < Qe){
                    if((A+C) > 0xffff){
                        carry=true;
                    }
                    C += A;
                    toInt16(C);
                    A = Qe;
                }
                currentState += BAC_StateTable[currentState][2];
                Qe = BAC_StateTable[currentState][1];
                BAC_Renormalize(A, C, carry, out);
            }
        }
        else{
            A -= Qe;
            if(A >= Qe){
                if(A + C > 0xffff){
                    carry = true;
                }
                C += A;
                toInt16(C);
                A = Qe;
            }
            currentState -= BAC_StateTable[currentState][3];
            if(BAC_StateTable[currentState][3] < 0){
                MPS = 1-MPS;
            }
            Qe = BAC_StateTable[currentState][1];
            BAC_Renormalize(A, C, carry, out);
        }
    }
}

void BAC_run(const char* name, unsigned int A, unsigned int C, int currentState, int MPS, const string& mapping){
    ifstream f(name, ios_base::binary);
    vector<unsigned char> v((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    string myOutput;
    string file_name = name;
    ofstream outfile("./"+file_name+"_BAC_encode.dat");
    long int size = FileSize(name);
    map<unsigned char, string> mp = myMapping(name, mapping);
    
    unsigned int Qe = BAC_StateTable[currentState][1];
    bool carry = false;
    if(mapping != "BP"){
        for(int i = 0; i < size; ++i){
            string bits;
            //bits += v[i];
            bits = mp[v[i]];
            BAC_main(bits, myOutput, A, C, Qe, currentState, MPS, carry);
    
            if(myOutput.length() >= 8){
                outfile<<Bit2Byte(myOutput);
            }
        }
    }
    else{
        vector<vector<char>> bits = BitPlane(name);
        for(int i = 0; i < 8; ++i){
            for(int j =0; j < bits.size(); ++j){
                string b;
                b = bits[j][i];
                BAC_main(b, myOutput, A, C, Qe, currentState, MPS, carry);
        
                if(myOutput.length() >= 8){
                    outfile<<Bit2Byte(myOutput);
                }
            }
        }
    }
    int x = 0;
    for(int j = 0; j < myOutput.length(); ++j){
        int t = (myOutput[j] == 0) ? 0 : 1;
        x += pow(2,7-j)*t;
    }
    outfile<<(char)x;
}

#endif /* BAC_hpp */
