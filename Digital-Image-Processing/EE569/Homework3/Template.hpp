//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  Template.hpp
//  EE569
//
//  Created by Alex on 2019/2/12.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef Template_hpp
#define Template_hpp

#include "HW3Basic.hpp"

//Masks for Morphological Processing

//conditional mask
const Vec1_s Cond_Mask({
        //1
            "101000000","100010000","100000100","100000001",
        //2
            "110000000","100100000","100001000","100000010",
        //3
            "111000000","101100000","100110000","100011000",
            "100001100","100000110","100000011","110000001",
        //4
            "110100000","100101000","100001010","110000010",
            "111000001","101110000","100011100","100000111",
        //5
            "110110000","110100001","101101000","111000010",
            "111100000","100111000","100001110","110000011",
        //6
            "110110001","101101100",
            "111110000","111100001","101111000","100111100",
            "100011110","100001111","110000111","111000011",
        //7
            "111110001","101111100","100011111","111000111",
        //8
            "111100011","111111000","100111110","110001111",
        //9
            "111110011","111100111","111111100","111111001",
            "101111110","100111111","110011111","111001111",
        //10
            "111110111","111111101","101111111","111011111",
        //11
            "111111011","111111110","110111111","111101111"
});

//conditional mask's lable
const Vec1_s Cond_Label({
        //1
             "S__","S__","S__","S__",
        //2
             "S__","S__","S__","S__",
        //3
             "S__","S__","S__","S__",
             "S__","S__","S__","S__",
        //4
             "_TK","_TK","_TK","_TK",
             "STK","STK","STK","STK",
        //5
             "ST_","ST_","ST_","ST_",
             "ST_","ST_","ST_","ST_",
        //6
             "ST_","ST_",
             "STK","STK","STK","STK",
             "STK","STK","STK","STK",
        //7
             "STK","STK","STK","STK",
        //8
             "STK","STK","STK","STK",
        //9
             "STK","STK","STK","STK",
             "STK","STK","STK","STK",
        //10
             "STK","STK","STK","STK",
        //11
             "__K","__K","__K","__K",
});

//unconditional mask for ST
const Vec1_s UnCond_MaskST({
        //Spur
            "101000000","100010000","100000010","110000000",
        //L Cluster
            "111000000","101100000","100110000","100011000",
            "100001100","100000110","100000011","110000001",
        //4-connected offset
            "101101000","110110000","110100001","111000010",
        //Spur corner cluster
            "111d00100","1d1100100","10011d001","100d11001",
            "1010011d0","10100d110","1100100d1","1d0010011",
        //Corner cluster
            "1dd111ddd",
        //Tee branch
            "1101d1d00","11d10100d","11d00101d","1100d1d10",
            "10d1d1010","100101d1d","110100d1d","11d1d0010",
        //Vee branch
            "1d1d1d1dd","1d1d1dd1d","1d1d1ddd1",
            "11dd1d1dd","1d1d1d1dd","1ddd1d1d1",
            "1d1ddd1d1","1dd1dd1d1","1ddd1d1d1",
            "1d1d1ddd1","1d1dd1dd1","1d1ddd1d1",
        //Diagonal branch
            "1101d010d","10d101d01","1010d101d","11d010d10"
});

//unconditional mask's label for ST
const Vec1_s UnCond_LabelST({
        //Spur
            "ST_","ST_","ST_","ST_",
        //Cluster
            "ST_","ST_","ST_","ST_",
            "ST_","ST_","ST_","ST_",
        //4-connected offset
            "ST_","ST_","ST_","ST_",
        //Spur corner cluster
            "ST_","ST_","ST_","ST_",
            "ST_","ST_","ST_","ST_",
        //Corner cluster
            "ST_",
        //Tee branch
            "ST_","ST_","ST_","ST_",
            "ST_","ST_","ST_","ST_",
        //Vee branch
            "ST_","ST_","ST_",
            "ST_","ST_","ST_",
            "ST_","ST_","ST_",
            "ST_","ST_","ST_",
        //Diagonal branch
        "ST_","ST_","ST_","ST_"
});

//unconditional mask for K
const Vec1_s UnCond_MaskK({
        //Spur
            "100000001","100000100",//"101000000","100010000",
        //Single 4-connection
            "100000010","110000000",//"100001000","100100000",
        //L corner
            "110100000","100101000","110000010","100001010",
        //Corner cluster
            "1dd111ddd","11ddddd11",
        //Tee branch
            "11d1d1ddd","1dd1d1d1d","11ddd1d1d","11d1ddd1d",
        //Vee branch
            "1d1d1d1dd","1d1d1dd1d","1d1d1ddd1",
            "11dd1d1dd","1d1d1d1dd","1ddd1d1d1",
            "1d1ddd1d1","1dd1dd1d1","1ddd1d1d1",
            "1d1d1ddd1","1d1dd1dd1","1d1ddd1d1",
        //Diagonal branch
            "1101d010d","10d101d01","1010d101d","11d010d10"
});

//unconditional mask's label for K
const Vec1_s UnCond_LabelK({
        //Spur
            "__K","__K","__K","__K",
        //Single 4-connection
            "__K","__K","__K","__K",
        //L corner
            "__K","__K","__K","__K",
        //Corner cluster
            "__K","__K",
        //Tee branch
            "__K","__K","__K","__K",
        //Vee branch
            "__K","__K","__K",
            "__K","__K","__K",
            "__K","__K","__K",
            "__K","__K","__K",
        //Diagonal branch
            "__K","__K","__K","__K",
});

#endif /* Template_hpp */
