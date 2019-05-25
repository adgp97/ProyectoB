%%
%Preliminaries
clear;clc;
%%
%Definition of the original signal

%Time vector
t = 0:0.00001:0.01;

%Signal
x1000 = sin(2*pi*10000*t);
x500 = sin(2*pi*5000*t);
x100 = sin(2*pi*100*t);
x = floor(46*(x1000 + x500 + x100));
%% Graficas
figure()
subplot(2,2,1)
plot(t,x1000);
subplot(2,2,2)
plot(t,x500);
subplot(2,2,3)
plot(t,x100);
subplot(2,2,4)
plot(t,x);

%%
%Frequency analysis
L = length(x) - 1;
x_fft = abs(fft(x));
x_fft = x_fft(1:(L/2+1));
f = 100000*(0:(L/2))/L;
figure()
plot(f,x_fft);

%%
%Filter

y = filter(Num,1,x);

figure()
plot(t,y);

%%
%Frequency analysis
L = length(y) - 1;
y_fft = abs(fft(y));
y_fft = y_fft(1:(L/2+1));
f = 100000*(0:(L/2))/L;
figure()
plot(f,y_fft);




