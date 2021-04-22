%  Yifan Wang
%  3038184983
%  wang608@usc.edu
%
%  compute_accuracy.m
%  EE569
%
%  Created by Alex on 2019/1/24.
%  Copyright © 2019 Alex. All rights reserved.
%

%  coumpute accuracy with each groundTruth

E = imread('Pig_canny_opp1600_5000_5.jpg');
load('Pig.mat');


[thrs1,cntR1,sumR1,cntP1,sumP1, V1] = edgesEvalImg( E, groundTruth(1));
[thrs2,cntR2,sumR2,cntP2,sumP2, V2] = edgesEvalImg( E, groundTruth(2));
[thrs3,cntR3,sumR3,cntP3,sumP3, V3] = edgesEvalImg( E, groundTruth(3));
[thrs4,cntR4,sumR4,cntP4,sumP4, V4] = edgesEvalImg( E, groundTruth(4));
[thrs5,cntR5,sumR5,cntP5,sumP5, V5] = edgesEvalImg( E, groundTruth(5));

[thrs,cntR,sumR,cntP,sumP, V] = edgesEvalImg( E, groundTruth);

[R,P,F]=computeRPF(cntR,sumR,cntP,sumP);
[bstR,bstP,bstF,bstT]=findBestRPF(thrs,R,P);

[R1,P1,F1]=computeRPF(cntR1,sumR1,cntP1,sumP1);
[bstR1,bstP1,bstF1,bstT1]=findBestRPF(thrs1,R1,P1);

[R2,P2,F2]=computeRPF(cntR2,sumR2,cntP2,sumP2);
[bstR2,bstP2,bstF2,bstT2]=findBestRPF(thrs2,R2,P2);

[R3,P3,F3]=computeRPF(cntR3,sumR3,cntP3,sumP3);
[bstR3,bstP3,bstF3,bstT3]=findBestRPF(thrs3,R3,P3);

[R4,P4,F4]=computeRPF(cntR4,sumR4,cntP4,sumP4);
[bstR4,bstP4,bstF4,bstT4]=findBestRPF(thrs4,R4,P4);

[R5,P5,F5]=computeRPF(cntR5,sumR5,cntP5,sumP5);
[bstR5,bstP5,bstF5,bstT5]=findBestRPF(thrs5,R5,P5);


