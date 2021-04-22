%  Yifan Wang
%  3038184983
%  wang608@usc.edu
%
%  parameter_test.m
%  EE569
%
%  Created by Alex on 2019/1/24.
%  Copyright © 2019 Alex. All rights reserved.
%

%  SPS          - [h x w] superpixel label map (S==0 are boundaries)
%  SPV          - [h x w] superpixel visualization
%  SPE          - [h x w] edge probability map multiscale=0
%  EDE          - [h x w] edge probability map multiscale=1
%  thrs       - [Kx1] vector of threshold values
%  cntR,sumR  - [Kx1] ratios give recall per threshold
%  cntP,sumP  - [Kx1] ratios give precision per threshold


%nPos_multiscale_sharpen_nTreesEval_nms

opts=edgesTrain();                % default options (good settings)
opts.modelDir='models/';          % model will be in models/forest
opts.modelFnm='modelBsds';        % model name
opts.nPos=5e5; opts.nNeg=5e5;     % decrease to speedup training
opts.useParfor=0;                 % parallelize if sufficient memory
tic, model=edgesTrain(opts); toc; % will load model if already trained
model.opts.multiscale=1;          % for top accuracy set multiscale=1
model.opts.sharpen=2;             % for top speed set sharpen=0
model.opts.nTreesEval=4;          % for top speed set nTreesEval=1
model.opts.nThreads=4;            % max number threads for evaluation
model.opts.nms=1;                 % set to true to enable nms
I = imread('Pig.jpg');

EDE=edgesDetect(I,model);
load('Pig.mat');

imshow(EDE);

[EDthrs,EDcntR,EDsumR,EDcntP,EDsumP] = edgesEvalImg( EDE, groundTruth);


