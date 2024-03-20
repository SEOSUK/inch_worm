%Run from editor Debug(F5)
%This M file constructs a low pass filter using the Z Transform and
%analyzes the characteristics of the filter such as frequency response and
%phase using the Matlab filter function with input vector x
%to get output y=filter(b,a,x).Frequency domain plots are also provided.
%The z transform is just basically a transformation from a linear system to
%a sampled system.
%The -3dB points of the filter can be set to <100Hz to >1GHz by appropriate
%setting of the sampling frequency(fs) and f. The purpose of constructing this M file was
%instigated by the following facts. I have some knowledge of Laplace
%transforms and the s plane. Someone mentioned using z transforms and I
%said "What is a z transform, that sounds complicated?". Anyway, that's why.
%I have added references at the end of the program that I have found
%helpful and hopefully playing with the program will give you a better
%understanding of the theory. Always remember to set your axis settings on
%plot routines.
%Low pass filter 1 pole 1 zero
%H(z)=z+1/z-p
%zero at -1
%pole at -1<x<1
%
%
%                    Z plane   !+j(imag axis)
%                              !
%                              !
%1 pole & 1 zero (-1)0---------!-------x--(+1)real axis
%                              !
%                              !
%                              !-j
%
%I should note that the H(z)=z/z-p transform also produces a low pass filter
%but is not as efficient as the one shown here. It has a zero at 0 and a
%pole from 0<x<1. It becomes a high pass filter with -1<x<0. You can
%observe this at the JDSP site using the pole-zero demo.
%Design example
%~.45Mhz -3dB point
%fs=1000000
clear
fs=1000000;%sample frequency
%f/fs=.5,f=500000
f=[0:5000:500000];%keep interval steps high for lower 
%computer processing time
Fn=fs/2;%nyquist frequency
w=2*pi*f/fs;%set so w=pi or 3.14
z=exp(w*j);%z definition
p=-.7;%change values for different filter -3dB bandwidths,(-.99 to .99)
gain=.85;%set gain for unity(1) with gain=(1-(p))/2 
Hz=gain*(z+1)./(z-(p));%z transform
%===================================================
%Magnitude and phase plots
%===================================================
figure(1)
subplot(3,1,1), plot(f,abs(Hz))
axis([0 1000000 0 1.5]);
grid on
title('Magnitude Response of H(z)')
ylabel('H(z)-voltage')
subplot(3,1,2);
Hz(Hz==0)=10^(-8);%avoid log(0)
plot(f,20*log10(abs(Hz)));
grid on
ylabel('dB')
axis([0 1000000 -3 1]);
subplot(3,1,3), plot(f,angle(Hz))
grid on
title('Phase Response of H(z)')
ylabel('radians*57.3=degrees')%one radian=57.3 degrees
xlabel('f [Hertz]')
axis([0 1000000 -2 0]);
fc=443000;%carrier frequency(Hz)
T=1/200;%sets span of y filter output plot
%x=A1*(cos(2*pi*fc*t)+phi)(cosine wave)
twopi_fc_t=(1:fs*T)*2*pi*fc/fs; 
A1=1;
phi=0;
x = A1 * cos(twopi_fc_t + phi);
%you could also use a square wave or an impulse signal insteat of a cosine
%wave.
%===============================================
%a and b filter coefficients
%===============================================
a=[1 .7];%(z-(p))
b=[.85 .85];%gain times [1 1)
% ===============================================
%Output y = filter(b,a,x)
% ===============================================
y = filter(b,a,x);
%plot input x and output y
%observe amplitude and phase shift of y compared to x. Will need
%much lower -3dB point filter(say 200Hz) and span to see phase shift. Notice 1Vp(input) to
%.7Vp(output) for -3dB point. Magnitude and phase shift at fc shown in
%command window.
figure(2);
plot(x,'b-'); hold on;
plot(y,'r-');
grid on
title('input x(blue)  /  output y(red)')
ylabel('Voltage')
hold off;
%Magnitude and phase at fc
w=2*pi*fc/fs;
z=exp(w*j);
Hz=gain*(z+1)./(z-(p));
magfc=abs(Hz)
phaseradfc=angle(Hz)
%===================================================
%Frequency domain plots
%====================================================
%Lets take a look at y output on a spectrum analyzer. This fft code is
%somewhat complex but comes from the mathworks. Works great as it shows
%one what they would actually see using a spectrum analyzer.
NFFY=2.^(ceil(log(length(y))/log(2)));
FFTY=fft(y,NFFY);%pad with zeros
NumUniquePts=ceil((NFFY+1)/2); 
FFTY=FFTY(1:NumUniquePts);
MY=abs(FFTY);
MY=MY*2;
MY(1)=MY(1)/2;
MY(length(MY))=MY(length(MY))/2;
MY=MY/length(y);
f1=(0:NumUniquePts-1)*2*Fn/NFFY;
figure(3)
subplot(2,2,1); plot(f1,MY);xlabel('FREQUENCY');ylabel('AMPLITUDE');
axis([0 .5e6 -1 1]);%zoom in/out
grid on;
subplot(2,2,2); plot(f1,10*log10(abs(MY).^2));xlabel('FREQUENCY');ylabel('20LOG10=DB');
axis([0 1e6 -60 5]);
grid on;
%observe that the carrier frequency(fc) is at .7 down -3dB on the plots as one would
%expect.
%References
%http://www.eas.asu.edu/~midle/jdsp/jdsp.html
%This site is one of the best. Log on and use the pole-zero demo and the
%filter demo. I was impressed.
%http://www.ece.uvic.ca/~peter/
%A good site for communication and filter theory and m files. Play close 
%attention to the pdf book notes and student project design of a bandpass 
%filter.