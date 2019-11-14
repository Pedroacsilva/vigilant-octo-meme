%CONTROLO DIGITAL
%AUTOR: Pedro Silva
%2019/2020
close all; clear all; clc;
Kp = linspace(0,1);
dois_a_zero = 1 - Kp.^2;
um_a_zero = dois_a_zero - (Kp.^2)./(dois_a_zero);
 alfa_1 = (-1 + Kp./dois_a_zero)./(um_a_zero);
zero_a_zero = um_a_zero - alfa_1 .* (- 1 + (Kp./dois_a_zero));
h = 1;
num = [h];
den = [1 -1 0 0];
H = tf(num, den);
fig1 = figure(1);
rlocus(H);
zgrid
axis equal

fig2 = figure(2);
%plot(Kp, um_a_zero, Kp, dois_a_zero); grid on; xlabel('K_p'); legend('^1a_0', '^2a_0', '^0a_0')
plot(Kp, dois_a_zero, 'r', Kp, um_a_zero, 'g', Kp, zero_a_zero, 'k');grid on; xlabel('K_p'); ylabel('^ia_j'); title('Coeficientes da matriz de Jury em função de K_p'); legend('^2a_0', '^1a_0', '^0a_0'); xlim([0 1]); ylim([-1 1])