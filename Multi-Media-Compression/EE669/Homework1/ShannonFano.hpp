//
//  ShannonFano.hpp
//  EE669
//
//  Modified by Alex on 2019/9/6.
//  Copyright © 2019 Alex. All rights reserved.
//
// From:
/*
 * Compress text file by using Shannon-Fano (No real compression,
 * just get the 0101 code)
 *
 * Author: bonep
 * Last update: 20120425
 *
 * Please free feel to use this code in any way
 *
 * Compile:
 * linux> c++ Shannon_Fano.cpp
 *
 * Usage:
 * linux> ./a.out
 * (the input text file is hardcoded as text1.txt)
 */

#ifndef ShannonFano_hpp
#define ShannonFano_hpp

#include "HW1_utli.hpp"

#define MAX_CHAR 256

using namespace std;

class SymbolCode {
    private:
        unsigned char symbol;
        int frequency;
        string code;
    public:
        SymbolCode( unsigned char in_symbol, int in_frequency );
        unsigned char getSymbol( void );
        int getFrequency( void );
        string getCode( void );
        void addCode( string in_code );
};

SymbolCode::SymbolCode( unsigned char in_symbol, int in_frequency ) {
    symbol = in_symbol;
    frequency = in_frequency;
    code = "";
}

unsigned char SymbolCode::getSymbol( void ) {
    return symbol;
}

int SymbolCode::getFrequency( void ) {
    return frequency;
}

string SymbolCode::getCode( void ) {
    return code;
}

void SymbolCode::addCode( string in_code ) {
    code += in_code;
}

vector<SymbolCode> symbol;
int frequency[ MAX_CHAR ];

void shannonFano( int in_begin, int in_end ) {
    if ( in_begin >= in_end ) {
        return;
    }
    if ( in_begin == in_end - 1 ) {
        symbol.at( in_begin ).addCode( "0" );
        symbol.at( in_end ).addCode( "1" );
        return;
    }
    
    int highPtr = in_begin;
    int lowPtr = in_end;
    int highSum = symbol.at( highPtr ).getFrequency();
    int lowSum = symbol.at( lowPtr ).getFrequency();
    
    while ( highPtr != lowPtr - 1 ) {
        if ( highSum > lowSum ) {
            lowPtr --;
            lowSum += symbol.at( lowPtr ).getFrequency();
        } else {
            highPtr ++;
            highSum += symbol.at( highPtr ).getFrequency();
        }
    }
    
    int i;
    for ( i=in_begin; i<=highPtr; i++ ) {
        symbol.at( i ).addCode( "0" );
    }
    for ( i=lowPtr; i<=in_end; i++ ) {
        symbol.at( i ).addCode( "1" );
    }
    
    shannonFano( in_begin, highPtr );
    shannonFano( lowPtr, in_end );
}

void SF_encode(const vector<unsigned char>& data, map<unsigned char, string> mp, const string& name){
    ofstream file("./"+name+"_SF_encode.dat");
    string tmp;
    for(int i = 0; i < data.size(); ++i){
        tmp = tmp+mp[data[i]];
        while(tmp.length() > 8){
            int len = (int)tmp.length();
            int x = 0;
            for(int j = 0; j < 8; ++j){
                int t = (tmp[j] == '0') ? 0 : 1;
                x += pow(2,7-j)*t;
            }
            
            file<<(unsigned char)x;
            tmp = tmp.substr(8, len);
        }
    }
    int x = 0;
    for(int j = 0; j < tmp.length(); ++j){
        int t = tmp[j] == 0 ? 0 : 1;
        x += pow(2,7-j)*t;
    }
    file<<(unsigned char)x;
}

map<unsigned char, string> SF_getDict(const char* name){
    FILE * pFile;
    int c;
    pFile = fopen(name,"rb");
    if (pFile==NULL) {
        perror ("Error opening file");
    } else {
        int i;
        for ( i=0; i<MAX_CHAR; i++ ) {
            frequency[ i ] = 0;
        }
        do {
            c = fgetc (pFile);
            if ( c != EOF ) {
                frequency[ c ] ++;
               // printf( "%c", c );
            }
        } while (c != EOF);
        fclose (pFile);
    }
    for (int i=0; i<MAX_CHAR; i++ ) {
        if ( frequency[ i ] != 0 ) {
            if ( symbol.empty() ) {
                SymbolCode newSymbol( i, frequency[ i ] );
                symbol.push_back( newSymbol );
            } else {
                int j;
                vector<SymbolCode>::iterator it;
                it = symbol.begin();
                for ( j=0; j<symbol.size(); j++ ) {
                    if ( frequency[ i ] >= symbol[ j ].getFrequency() ) {
                        SymbolCode newSymbol( i, frequency[ i ] );
                        symbol.insert( it, newSymbol );
                        break;
                    } else if ( j == symbol.size() - 1 ) {
                        SymbolCode newSymbol( i, frequency[ i ] );
                        symbol.push_back( newSymbol );
                        break;
                    }
                    it ++;
                }
            }
        }
    }
    shannonFano( 0, (int)symbol.size() - 1 );
    map<unsigned char, string> mp;
    double len = 0;
    double ct = 0;
    //cout<<"  char    code"<<endl;
    for(int i = 0; i < symbol.size(); ++i){
        mp[symbol[i].getSymbol()] = symbol[i].getCode();
        //cout <<"   "<<(int)symbol[i].getSymbol()<<"  "<< symbol[i].getCode() << endl;
        len += ((int)symbol[i].getCode().length())*symbol[i].getFrequency();
        ct += symbol[i].getFrequency();
    }
    //cout<<"Average length: "<<len/ct<<endl;
    return mp;
}

void SF_run(const char* name){
    map<unsigned char, string> mp = SF_getDict(name);
    ifstream f(name, ios_base::binary);
    vector<unsigned char> v((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    SF_encode(v, mp, (string)name);
}

/*
int SF_Separate(const vector<double>& freq, int start, int end){
    double left = 0;
    double right = 0;
    end = end - 1;
    while(start < end){
        if(left > right){
            right += freq[end];
            end--;
        }
        else if(left < right){
            left += freq[left];
            start++;
        }
        else{
            left += freq[left];
            start++;
        }
    }
    return start;
}

void SF_Assign(vector<string>& code, int start, int end, int sep){
    int i = start;
    for(; i <= sep; ++i){
        code[i] += '0';
    }
    for(; i < end; ++i){
        code[i] += '1';
    }
}

map<unsigned char, string> SF_convert(const vector<unsigned char>& label, const vector<string>& code){
    map<unsigned char, string> mp;
    for(int i = 0; i < label.size(); ++i){
        mp[label[i]] = code[i];
    }
    return mp;
}

map<unsigned char, string> SF_CodingDict(const vector<unsigned char>& data){
    vector<double> freq;
    vector<unsigned char> dict;
    GetStatics(data, freq, dict);
    vector<string> code(freq.size());
    queue<vector<int>> sk;
    sk.push({0, (int)freq.size()});
    while(!sk.empty() == true){
        int sep = SF_Separate(freq, sk.front()[0], sk.front()[1]);
        SF_Assign(code, sk.front()[0], sk.front()[1], sep);
        if(sep > sk.front()[0]){
            sk.push({sk.front()[0], sep});
        }
        if(sep+1 < sk.front()[1]){
            sk.push({sep, sk.front()[1]});
        }
        sk.pop();
    }
    cout<<"Shannon Fano encoding dict:"<<endl;
    cout<<"  "<<"unsigned char"<<"  "<<"code"<<endl;
    for(int i = 0; i < freq.size(); ++i){
        cout<<"    "<<(int)dict[i]<<"     "<<code[i]<<endl;
    }
    cout<<"Average code length: "<<AvgCodeLength(freq, code)<<endl;
    return SF_convert(dict, code);
}

void SF_encode(const vector<unsigned char>& data, map<unsigned char, string> mp, const string& name){
    ofstream file("./"+name+"_SF_encode.dat");
    string tmp;
    for(int i = 0; i < data.size(); ++i){
        tmp = tmp+mp[data[i]];
        while(tmp.length() > 8){
            int len = (int)tmp.length();
            int x = 0;
            for(int j = 0; j < 8; ++j){
                int t = (tmp[j] == '0') ? 0 : 1;
                x += pow(2,7-j)*t;
            }
            
            file<<(unsigned char)x;
            tmp = tmp.substr(8, len);
        }
    }
    int x = 0;
    for(int j = 0; j < tmp.length(); ++j){
        int t = tmp[j] == 0 ? 0 : 1;
        x += pow(2,7-j)*t;
    }
    file<<(unsigned char)x;
}

void SF_run(const string& name){
    ifstream f(name, ios_base::binary);
    vector<unsigned char> v((istreambuf_iterator<unsigned char>(f)), istreambuf_iterator<unsigned char>());
    if(v.size() <= 0){
        cout<<"Can not read File."<<endl;
        return;
    }
    map<unsigned char, string> mp = SF_CodingDict(v);
    SF_encode(v, mp, name);
}
*/
#endif /* ShannonFano_hpp */
