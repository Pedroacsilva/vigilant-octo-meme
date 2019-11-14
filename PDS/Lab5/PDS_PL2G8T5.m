%Lab 5 PDS: Banco de Filtros de 3 Canais
%PL2
%Grupo 8
%Autores:
%Pedro Carrinho
%Pedro Silva
close all; clc; clear ; mkdir('Imagens '); delete Imagens/*.*

% 1b) Determine o atraso D1 de forma a existir reconstrução perfeita.
% R: c0[n] e c1[n] demonar 35 amostras a serem calculadas (ie atraso 35 amostras)
% e c2[n] deverá entrar no ultimo filtro de sintese ao mesmo tempo que c0 e c1,
% logo D1 deverá ser igual a D, assim D = D1 = 35. Acho que intuitivamente
% percebo que o atraso global tenha que ser 105, mas nao sei, estou
% completamente as aranhas e dou esta resposta apos ter feito plot de y


% 1c) Considere um sinal de áudio de entrada. Use as funções implementadas no trabalho anterior, bem
% como a função delayCh, para calcular as saídas dos 3 canais, c0[n], c1[n] e c2[n]. Ouça os sinais
% dos vários canais às frequências de amostragem corretas. Comente.

%Load de filtros criados na aula laboratorial 4
load('filtros.mat');
[x,fs] = audioread('pcmtest.wav'); % lê sinal e freq. de amostragem.
[x0,x1] = Analysis2(h0, h1, x);
[c0,c1] = Analysis2(h0,h1, x0);
c2 = delayCh(x1, 35);           %aplicar um atraso de 35 amostras para reconstrucao perfeita
y = filter(f0, 1, upsample(Synthesis2(f0,f1, c0, c1), 2)) + filter(f1, 1, upsample(c2,2));
fig1 = figure(1);
plot(1:length(x), x,1:length(y), y, 'r'); grid on; title('Sinal de entrada e filtrado'); xlabel('n'); ylabel('x[n]'); xlim([0 200]); legend({'Sinal de entrada', 'Sinal filtrado'})
print(fig1, '-dpng', 'Imagens/sinal_entrada_saida')
%Verificamos um atraso global de 105 amostras
fig2 = figure(2);
plot(1:length(x)-106, x(1:end-106), 1:length(y)-105, y(106:end), 'r'); grid on; title('Sinais de entrada e de saída sobrepostos (atraso compensado)'); xlim([0 200]); legend({'Sinal de entrada', 'Sinal filtrado'})
print(fig2, '-dpng', 'Imagens/sinal_entrada_saida_atraso_compensado')
disp('Maior erro de construção tendo em conta o atraso')
max(abs(x(1:end-106) - y(106:end-1)))       %Verificamos que o erro de reconstrução é de ordem de grandeza muito pequena, pode ser desprezado
% 1e) Reconstrua o sinal (num vetor y2) desprezando o canal c2[n]. Ouça os sinais contidos em y e y2
% e detete a diferença. Ouça também a diferença y-y2 para ter uma ideia do que se perdeu.
y2 = filter(f0, 1, upsample(Synthesis2(f0,f1, c0, c1), 2));
fig3 = figure(3);
plot(1:length(y), y, 1:length(y2), y2, 'r'); grid on; title('Sinais de saida com e sem o canal 2'); legend({'Sinal com canal 2', 'Sinal sem canal 2'})
print(fig3, '-dpng', 'Imagens/sinal_com_c2_sem_c2')
%Análise do espectro de y e y2
%https://www.mathworks.com/help/releases/R2019a/matlab/ref/fft.html?container=jshelpbrowser
FFTY = fft(y);
FFTY2 = fft(y2);
yP2 = abs(FFTY/length(y));
yP1 = yP2(1:length(y)/2+1);
yP1(2:end-1) = 2*yP1(2:end-1);
f = fs*(0:(length(y)/2))/length(y);
fig4 = figure(4);
subplot(2,1,1)
plot(f,yP1); grid on; title('Espectro de y[n]'); xlabel('f (Hz)');
y2P2 = abs(FFTY2/length(y2));
y2P1 = y2P2(1:length(y2)/2+1);
y2P1(2:end-1) = 2*y2P1(2:end-1);
subplot(2,1,2)
plot(f,y2P1); grid on; title('Espectro de y2[n]'); xlabel('f (Hz)');
print(fig4, '-dpng', 'Imagens/espectro_y_y2')
disp('Observando o espectro de y e de y2, observamos que em y2 perdemos as componentes com frequência superior a 4500 Hz')
disp('Sinal de saida com c2')
sound(y, fs)
pause(3)
disp('Sinal de saida sem c2')
sound(y2, fs)
pause(3)
disp('Diferença entre os dois sinais (y-y2)')
sound(y-y2, fs)
pause(3)
disp('c0')
sound(c0, fs/4)
pause(3)
disp('c1')
sound(c1, fs/4)
pause(3)
disp('c2')
sound(c2, fs/2)
pause(3)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%               Parte 2. Resposta em frequencia dos 3 canais              %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 2b) Calcule as respostas em frequência dos dois filtros H0(z) e H1(z) do banco de filtros base, usando
% DFTs de 2048 pontos (use variáveis com nomes H0 e H1). Para obter a resposta em frequência de,
% por exemplo, H0(z2), verifique, que, através das propriedades, basta contrair por um fator de 2 a
% resposta de H0(z). Assim, basta fazer H02=[H0(1:2:end),H0(1:2:end)] para ter a resposta de
% H0(z2) no intervalo [0,2pi[ com o mesmo número de pontos de H0. No tempo, equivale a expandir
% h0[n] por 2.
% Defina um vetor de frequências de 1025 pontos de forma a fazer um “plot” do módulo das respostas
% em frequência dos filtros de análise, G0, G1 e G2, no intervalo normalizado [0, fs/2].
% Faça um “plot” em simultâneo do módulo da resposta em frequência dos 3 canais. Coloque uma
% legenda no gráfico para identificar as respostas.
H0 = fft(h0, 2048);
H1 = fft(h1, 2048);
frequencias = (0:(fs/2)/1024:(fs/2));
H02 = [H0(1:2:end), H0(1:2:end)];
H12 = [H1(1:2:end), H1(1:2:end)];
G0 = H0 .* H02;
G1 = H0 .* H12;
G2 = H1;
fig5 = figure(5);
k = 1:1025;        %auxiliar para iterar sobre valores dos filtros
% plot(1:length(H0), abs(H0), 1:length(H1), abs(H1), 'r'); grid on; xlim([0 length(H0)]); legend({'|H_0(z)|', '|H_1(z)|'})
plot(frequencias, abs(H0(k)), frequencias, abs(H1(k)), 'r'); grid on; xlabel('F (Hz)'); ylabel('|H_n|'); title('Ganho em frequência dos filtros H_0 e H_1'); xlim([0 frequencias(end)]); legend({'|H_0(z)|', '|H_1(z)|'})
print(fig5, '-dpng', 'Imagens/ganho_freq_H0_H1')
% plot(frequencias, abs(H0(1:2:end)), frequencias, abs(H1(1:2:end)), 'r'); grid on; xlabel('F (Hz)'); ylabel('|H_n|'); title('Ganho em frequência dos filtros H_0 e H_1'); xlim([0 frequencias(end)]); legend({'|H_0(z)|', '|H_1(z)|'})
fig6 = figure(6);
plot(frequencias, abs(G0(k)), frequencias, abs(G1(k)), 'r', frequencias, abs(G2(k)), 'm'); grid on; xlabel('F (Hz)'); ylabel('|G_n|'); title('Ganho em frequência de G_0, G_1, G_2'); xlim([0 frequencias(end)]); legend({'|G_0(z)|', '|G_1(z)|', '|G_2(z)|'})
print(fig6, '-dpng', 'Imagens/ganho_freq_G0_G1_G2')
% plot(frequencias, abs(G0(1:2:end)), frequencias, abs(G1(1:2:end)), 'r', frequencias, abs(G2(1:2:end)), 'm'); grid on; xlabel('F (Hz)'); ylabel('|G_n|'); title('Ganho em frequência dos filtros H_0 e H_1'); xlim([0 frequencias(end)]); legend({'|G_0(z)|', '|G_1(z)|', '|G_2(z)|'})


% 2c) O que perdemos em 1e)?
%Perdemos as componentes de frequencia superior a 4 kHz
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%               Parte 3. Resposta a impulso dos 3 canais              	  %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 3a) Calcule as 3 respostas a impulso dos associadas aos 3 canais do banco de filtros, [] i gn, i=0,1,2,
% usando a expansão de h0[n] e h1[n] por 2 para obter as respostas a impulso de 2
% 0 ()Hz e 2
% 1 ()Hz .
% Use a função conv(). Verifique que as respostas g0[n] e g1[n] têm comprimento 3D+2 enquanto
% g2[n] tem comprimento D+1. Explique porquê.
%alocacao de memoria
h0_2 = zeros(1, 2 * length(h0));
h1_2 = zeros(1, 2 * length(h1));
h0_2(1:2:end) = h0;
h1_2(1:2:end) = h1;
%calculo de g's
g0 = conv(h0, h0_2);
g1 = conv(h0, h1_2);
g2 = h1;
disp('g0 tem tamanho: ')
length(g0)
disp('g1 tem tamanho: ')
length(g1)
disp('g2 tem tamanho: ')
length(g2)
%3b) Verifique que calculou bem g1[n], comparando a sua DFT (com 2048 pontos) com G1.
max(abs(fft(g1,2048) - G1))
%O erro entre a DFT de g1 e G1 é devido à precisão numérica e pode ser
%desprezado.


