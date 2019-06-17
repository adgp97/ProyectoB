%% Preliminaries
clear;clc;
%% 

t = 0:0.00001:0.01;
w10000 = 2*pi*10000;
w5000 = 2*pi*5000;
w100 = 2*pi*100;
x10000 = zeros(1,1001);
x5000 = zeros(1,1001);
x100 = zeros(1,1001);
i = 1;
while (1)
    x10000(1001) = sin(w10000*t(i));
    x5000(1001) = sin(w5000*t(i));
    x100(1001) = sin(w100*t(i));
    
    if (i<1001)
        i = i + 1;
    else
        i = 1;
    end
    
    for j = 1:1000
       x100(j) = x100(j+1);
       %x5000(j) = x5000(j+1);
       %x10000(j) = x10000(j+1);
    end
    drawnow;
    %subplot(3,1,1);
    plot(t,x100);
    %subplot(3,1,2);
    %plot(t,x5000);
    %subplot(3,1,3);
    %plot(t,x10000);
    
end