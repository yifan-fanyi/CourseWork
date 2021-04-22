
function [image] = readraw(filename, H, W)
f1 = fopen(filename, 'r');
data = fread(f1, 'uint8');
fclose(f1);
len = length(data);
k = len/(H*W);
image = reshape(data, H, W, k);
return
