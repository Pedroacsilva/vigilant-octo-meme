%CONTROLO DIGITAL
%AUTOR: Pedro Silva
%2019/2020
close all; clear all; clc; mkdir('Imagens ');delete Imagens/*.*
s = tf('s');
%Dados do enunciado. G(s) = K * 1/(s(s+1)), K = 0.391
num = [1];
den =[1 1 0];
G = tf(num, den);
K = 0.391;
%Discretizar o controlador
num_d = [0.3679, 0.2642];
den_d = [1 -1.3679, 0.3679];
G_d = tf(num_d, den_d);
%Obter espaco de estados atraves da TF
[A, B, C, D] = tf2ss(num, den);
%Queremos analisar as variaveis de estado (vector X), portanto usar estes
%matrizes de estado auxiliares.
C_2 = [1 0; 0 1];
D_2 = [0; 0];
%Discretizar o SS para h = 1s
h = 1;
[phi, gama] = c2d(A,B, h);
%simular
simulacao = sim('sim5_3.slx');
%Plot das saidas do sistema
fig1 = figure(1);
plot(simulacao.y_continuo.time, simulacao.y_continuo.signals.values, 'r', simulacao.y_continuo_SS.time, simulacao.y_continuo_SS.signals.values, 'g', simulacao.y_continuo_SS2.time, simulacao.y_continuo_SS2.signals.values, 'b', simulacao.y_discreto_SS.time, simulacao.y_discreto_SS.signals.values, 'y', simulacao.y_discreto.time, simulacao.y_discreto.signals.values, 'm'); grid on
xlabel('t'); ylabel('y'); legend('Saida continua', 'Saida continua SS', 'Saida continua SS2', 'Saida Discreta SS', 'Saida Discreta');
print(fig1, '-dpng', 'Imagens/Saidas_do_sistema')
fig2 = figure(2);
plot(simulacao.y_continuo.time, simulacao.y_continuo.signals.values, 'r', simulacao.y_continuo_SS.time, simulacao.y_continuo_SS.signals.values, 'g', simulacao.y_continuo_SS2.time, simulacao.y_continuo_SS2.signals.values, 'b');
grid on; xlabel('t'); ylabel('y'); legend('Saida continua', 'Saida continua SS', 'Saida continua SS2');
fig3 = figure(3);
plot(simulacao.y_discreto_SS.time, simulacao.y_discreto_SS.signals.values, 'y', simulacao.y_discreto.time, simulacao.y_discreto.signals.values, 'm');
grid on; xlabel('t'); ylabel('y'); legend('Saida Discreta SS', 'Saida Discreta');