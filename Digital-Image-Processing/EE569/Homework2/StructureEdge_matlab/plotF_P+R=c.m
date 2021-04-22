%  Yifan Wang
%  3038184983
%  wang608@usc.edu
%
%  plotF_P+R=c.m
%  EE569
%
%  Created by Alex on 2019/1/24.
%  Copyright © 2019 Alex. All rights reserved.
%

%  plot F when P+R=const

x=[0.001:0.001:1];
for i = 1:1000
        z=2*x(i)*(1-x(i));
        hold on;
        plot(x(i),z,'.-');
end

z0=2*x.*(0.7-x);
z1=2*x.*(1-x);
z2=2*x.*(1.5-x);
z3=2*x.*(1.9-x);
plot(x,z0,'-');
hold on;
plot(x,z1,'-');
hold on;
plot(x,z2,'-');
hold on;
plot(x,z3,'-');
set(gca,'XLim',[0 1]);
set(gca,'YLim',[0 1]);
legend('P+R=0.7','P+R=1','P+R=1.5','P+R=1.9');
