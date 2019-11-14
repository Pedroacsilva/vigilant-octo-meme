%CD; Ex 3.3 p.72
%Autor: Pedro Silva
close all; clc; clear; mkdir('Imagens '); delete Imagens/*.*
%Função de transferencia do sistema a controlar
s = tf('s');        %TF auxiliar
G = 1/(s+1);
[numG, denG] = tfdata(G, 'v');      %Extrair o TF
%TF do controlador PI analogico
ksi = 0.7; wn = 5;
%ganhos para o PI analogico
Kp_a = 6;
Ki_a = 25;
Gc = (Kp_a * s + Ki_a)/s;
[numGc, denGc] = tfdata(Gc, 'v');      %Extrair o TF
%PI DISCRETO, sample period h = 0.1s
%Método para discretizar: Trapezoidal (s = (2/h)*(z-1)/(z+1))
Kp_d = Kp_a;            %O ganho proporcional não muda
h = 0.1;
Ki_d = Ki_a * h / 2;

%Correr simulação
simulacao = sim('sim3_3.slx');
fig1 = figure(1);
plot(simulacao.y_analog.time, simulacao.y_analog.signals.values); grid on; title('Saída do sistema controlado por PI analógico'); xlabel('t'); ylabel('y');
print(fig1, '-dpng', 'Imagens/Saida_PI_Analogico')
hold on;
plot(simulacao.y_disc.time, simulacao.y_disc.signals.values); grid on; title('Saída do sistema controlado por PI discreto (h = 0.1s)'); xlabel('t'); ylabel('y');
%Correr simulacao para h = 0.2s
h = 0.2;
Ki_d = Ki_a * h / 2;
simulacao = sim('sim3_3.slx');
plot(simulacao.y_disc.time, simulacao.y_disc.signals.values); legend('y(t) PI analógico', 'y(t) h = 0.1s', 'y(t) h = 0.2s');







