%%
%Preliminaries
clear;clc;
%%
Fs = 1000;
T = 1/Fs;
L = 1500;
t = (0:L-1)*T;

%Original signal
S = 0.7*sin(2*pi*50*t) + sin(2*pi*120*t);

%Signal + Noise
X = S + 2*randn(size(t));

plot(t,X);

%Fourier Transform of S
Y = fft(S);

P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);

f = Fs*(0:(L/2))/L;
plot(f,P1);