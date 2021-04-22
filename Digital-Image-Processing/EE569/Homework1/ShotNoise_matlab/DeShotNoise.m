%  Yifan Wang
%  3038184983
%  wang608@usc.edu
%
%  DeShotNoise.m
%  EE569
%
%  Created by Alex on 2019/1/7.
%  Copyright © 2019 Alex. All rights reserved.
%

fid=fopen('pepper_dark_noise.raw','rb');
pixel = fread(fid, inf, 'uchar');
fclose(fid);
Noise = reshape(pixel, 256, 256);

tmp = 2*sqrt(Noise+ 0.375);
m1 = max(Noise(:));  
m2 = 2*sqrt(0+3/8); 
tmp=(tmp-m2)/(m1-m2);
    
[x, res] = BM3D(1, tmp, 1.85, 'np', 0); 

res = res*(m1-m2)+m2;
res = res.^2/4-0.125;

fid = fopen('SN1_75.raw','wb');
count = fwrite(fid, res, 'uchar');
fclose(fid);


