%CONTROLO DIGITAL
%AUTOR: Pedro Silva
%2019/2020
%Problema 7.2
close all; clear all; clc; mkdir('Imagens ');delete Imagens/*.*


A = [0 1; -2 -3]; B = [0; 4]; C = [1 0]; D = 0;
Ca=[1 0;0 1];Da=[0; 0];
%Calcular phi(t):
%Descobrir phi(s) = inv((s*I - A));
syms s;
phi_s = inv((s*eye(2) - A));
phi_t = ilaplace(phi_s);

% temos phi_t = [   2*exp(-t) - exp(-2*t),   exp(-t) - exp(-2*t)]
% [ 2*exp(-2*t) - 2*exp(-t), 2*exp(-2*t) - exp(-t)]
h = 0.1;
phi_h = [   2*exp(-h) - exp(-2*h),   exp(-h) - exp(-2*h);
2*exp(-2*h) - 2*exp(-h), 2*exp(-2*h) - exp(-h)];
% O urbano ja da o gamma, mas para o calcular tinhamos de integrar phi_h de
% 0 a 0.1 e multiplicar por B.

gamma = [0.0181; 0.3444];

%Ok estou a ser incrivelmente estupido. O c2d existe
[phi, gamma] = c2d(A, B, h);

% Os 2 polos do sistema z(1,2) sao a +/- j*b.
% o polinomio caracteristico e entao (z - a - j * b) * (z -a + j * b)
% = z^2 -2a*z + (a^2 + b^2)


%Nos queremos polos em z = 0.4 +/- j *0.3
a=0.4; b=0.3;
% b=0; % para pólo duplo em z=0.4
p1=-2*a; p2=a^2+b^2;
% Matriz de controlabilidade completa
Wc=[gamma phi*gamma];
%phi_cl = (phi - gamma * L)
% Como calcular o L?
% L = [0 ... 1] * Wc^-1 * P(phi), onde phi vem do teorema de cayley-hamilton
L = [0 1] * inv(Wc) * (phi^2 + p1 * phi + p2*eye(2));
%ou equivalente
%L = acker(phi, gamma, [0.4 + i * 0.3, 0.4 - i * 0.3])
phi_cl = phi - gamma * L;

%Falta-nos ainda calcular o Lc
Lc = 1/(C*inv(eye(2)-phi_cl)*gamma);

simulacao = sim('sim7_2');
fig1 = figure(1);
%Acho que este plot esta incorrecto
plot(simulacao.yc.time, simulacao.yc.signals.values, 'b', simulacao.yc1.time, simulacao.yc1.signals.values, 'r'); grid on;xlabel('t'); ylabel('Amplitude'); title('Saida do sistema sem acesso as SV e com acesso as SC');



