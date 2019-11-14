%CONTROLO DIGITAL
%AUTOR: Pedro Silva
%2019/2020
close all; clear all; clc;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Jm = 0.05; Ra = 0.5; La = 10e-3; Ke = 0.4; Kf = 0.001;
%PID, introduzindo um polo nao dominante e varia-lo no eixo real (-100 a
%100)
p = -100:0.1:100;
ksi = 1; wn = 5; 
%Funcao de transferencia de motor DC
s = tf('s');
H = Ke/((Ra+s*La)*(s*Jm+Kf)+Ke^2);
[numH, denH] = tfdata(H, 'v');
%Normalizar a funçao de transferencia
numH = numH./denH(1);
denH = denH./denH(1);
%Cálculo de Kp, Ki
Kp = (2 * p * ksi * wn + wn^2 - denH(3))/numH(3);
Ki = (p * wn^2)/numH(3);
Kd = (2 * ksi * wn + p - denH(2))/numH(3);
% simulacao = sim('DC_PM_motor.slx');
%PLOT DAS CONSTANTES PID DE ACORDO COM P
fig1 = figure(1);
plot(p, Kp, 'r', p, Ki, 'm', p, Kd, 'y'); grid on; title('Constantes do PID'); xlim([-100 100]); xlabel('p'); ylabel('K_x'); legend('Kp', 'Ki', 'Kd');

%Vamos trabalhar com um pólo não dominante em 70 (p = -70)
p = 40.02;
Kp = (2 * p * ksi * wn + wn^2 - denH(3))/numH(3);
Ki = (p * wn^2)/numH(3);
Kd = (2 * ksi * wn + p - denH(2))/numH(3);
fig = figure(2);
GH = tf(Kp *(denH) + Ki /s * denH + Kp * s + denH + Ke, denH);
rlocus(H);
% %PI: Trabalhar com K_crit = 5; w_crit = 1
% K_crit = 5; w_crit = 1;
% P_crit = 2 * pi / w_crit;
% Kp_PI = 0.45 * K_crit;
% Ki_PI = (1/12) * P_crit;
%PID
Kp = (2 * p * ksi * wn + wn^2 - denH(3))/numH(3);
Ki = (p * wn^2)/numH(3);
Kd = (2 * ksi * wn + p - denH(2))/numH(3);
%Correr simulacao
sim = sim('DC_PM_motor');
