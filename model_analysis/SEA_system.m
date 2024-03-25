%%Series Elastic Actuator  

   %                      c1                    c2
   %    k1       |======|--]--        |======|--]--
   %--/-/-/-/-/--|  j1  |--/-/-/-/-/--|  j2  |
   %             |======|     k2      |======|
   % ______________________________________

% joint :: O 
% EE :: X
% 
% O---------O------------X
% |<------->|<----------->
%     L1          L2


% link value
L1 = 0.20075; %m
L2 = 0.149;   %m
m1 = 0.340;   %kg
m2 = 0.140;   %kg
mEE = 0.07;   %kg
CoM1 = 0.117; %m
CoM2 = 0.045; %m

%SEA value
k1 = 2.9*10^3; %N/m
k2 = 2.9*10^3; %N/m

c1 = 0.05; %ma.um.dea.ro value
c2 = 0.05; %ma.um.dea.ro value

J1 = m1*L1^2/12 + m1*CoM1^2;                     %kgm^2
J2 = m2*L2^2/12 + m2*CoM2^2 + (mEE*(L2-CoM2)^2); %kgm^2

%ref value
Fx = 0;
Fy = 0;
Px = 0.20075;
Py = 0.149;

%MCG value
g = 9.81;
N1 = m1*CoM1^2 + J1 + m2*L1^2 + m2*CoM2^2 + J2;
N2 = m2*L1*CoM2;
N3 = m2*CoM2^2 + J2;

%% Control section

% PID gain
k_p = 1;
k_d = 0.1;

%Transfer function
num = [0 k_d*j k_d*c+j*k_p c*k_p-k_d*k_sp -k_sp*k_p];
den = [J*j J*c+j*C C*c-J*k_sp-j*k_sp -c*k_sp-C*k_sp 0]+[0 k_d*j k_d*c+j*k_p c*k_p-k_d*k_sp -k_sp*k_p];

tf(num,den)

%stabillity
rlocus(num, den)

