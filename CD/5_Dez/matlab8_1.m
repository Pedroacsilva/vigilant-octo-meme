%CONTROLO DIGITAL
%AUTOR: Pedro Silva
%2019/2020
%Problema 8.1
close all; clear all; clc; mkdir('Imagens ');delete Imagens/*.*

Csim = eye(2);
Dsim = [0; 0];
%Queremos que o modelo continuo equivalente, em malha fechada, tenha o
%polinomio caracteristico s^2 + 1.8s + 1.25.
a = [1 1.8 1.25];
wnh = 0.4; G_dc = 1;
phi_12 = 1.5;
phi =[1 phi_12; 0 0.4];
gamma = [2; 1];
C = [1 0]; D = [0];
sys = ss(phi, gamma, C, D, []);
H = tf(sys);
wn = sqrt(a(3)); ksi = a(2)/(2*wn); h = wnh / wn;

p1 = -2 * exp(-ksi * wnh) * cos(wnh * sqrt(1 - ksi^2));      %polos caracteristicos
p2 = exp(-2 * ksi *wnh);



Wc = [gamma phi*gamma];           %matriz controlabilidade
L = [0  1] * inv(Wc) * (phi^2 + p1 * phi + p2 * eye(2));

phi_cl = phi - gamma * L;
polos_cl = eig(phi_cl);

Lc = G_dc / (C * inv(eye(2) - phi_cl) * gamma);

sys_cl = ss(phi_cl, gamma, C, D, []);
H_c = tf(sys_cl);
H_cl = Lc * H_c;

C2 = [0 1];

simulacao = sim('sim8_1.slx');
fig1 = figure(1);
plot(simulacao.yk.time, simulacao.yk.signals.values, 'k*', simulacao.x2k.time, simulacao.x2k.signals.values, 'ko'); grid on; xlabel('k'); ylabel('amplitude'); title('phi_{1,2} = 1.5');
%phi12 = -1.5
phi_12 = -1.5; phi =[1 phi_12; 0 0.4];sys = ss(phi, gamma, C, D, []); H = tf(sys);
Wc = [gamma phi*gamma];           %matriz controlabilidade
L = [0  1] * inv(Wc) * (phi^2 + p1 * phi + p2 * eye(2));
phi_cl = phi - gamma * L; polos_cl = eig(phi_cl); Lc = G_dc / (C * inv(eye(2) - phi_cl) * gamma); sys_cl = ss(phi_cl, gamma, C, D, []);
H_c = tf(sys_cl);
H_cl = Lc * H_c;
simulacao = sim('sim8_1.slx');
fig2 = figure(2);
plot(simulacao.yk.time, simulacao.yk.signals.values, 'k*', simulacao.x2k.time, simulacao.x2k.signals.values, 'ko'); grid on; xlabel('k'); ylabel('amplitude'); title('phi_{1,2} = -1.5');

