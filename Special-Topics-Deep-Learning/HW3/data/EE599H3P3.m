z = zeros(50,2);
z1 = zeros(50,2);
z2 = zeros(50,2);

for i = 1:250
    z(mod(i,50)+1,1) = z(mod(i,50)+1,1)+x(i,1);
    z(mod(i,50)+1,2) = z(mod(i,50)+1,2)+x(i,3);
end
for i = 1:200
    z1(mod(i,50)+1,1) = z1(mod(i,50)+1,1)+x1(i,1);
    z1(mod(i,50)+1,2) = z1(mod(i,50)+1,2)+x1(i,3);
end
for i = 1:300
    z2(mod(i,50)+1,1) = z2(mod(i,50)+1,1)+x2(i,1);
    z2(mod(i,50)+1,2) = z2(mod(i,50)+1,2)+x2(i,3);
end
z=z/5;
z1=z1/4;
z2=z2/6;
plot([2:50],z(2:50,1),'-');
hold on;
plot([2:50],z(2:50,2),'-');
hold on;
plot([2:50],z1(2:50,1),'-');
hold on;
plot([2:50],z1(2:50,2),'-');
hold on;
plot([2:50],z2(2:50,1),'-');
hold on;
plot([2:50],z2(2:50,2),'-');
legend('Adam_{train}','Adam_{val}','SGD_{train}', 'SGD_{val}', 'RMSprop_{train}','RMSprop_{val}');