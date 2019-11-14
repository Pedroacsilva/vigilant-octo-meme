%Lab 8 PDS: Processos Estocásticos. Estimativa da autocorrelação.
%PL2
%Grupo 8
%Autor:
%Pedro Silva
close all; clc; clear; mkdir('Imagens '); delete Imagens/*.*
% Um processo estocástico de Bernoulli é um processo x[n] discreto que toma apenas dois valores.
% Esses dois valores podem ser -1 e 1 (caso de processo de média zero) ou 0 e 1, ou outro par de valores.
% Neste trabalho vamos considerar x[n] com valores lógicos, verdade=1 e falso=0. O processo é
% definido com um único parâmetro: p = Pr (x[n]=1) . Significa que
% Pr(x[n]=0)=1-p
%https://en.wikipedia.org/wiki/Bernoulli_distribution
%http://mathworld.wolfram.com/BernoulliDistribution.html
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 1. Processo de Bernoulli
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
lena = imread('lena.tif');
fig1 = figure(1);
imshow(lena); title('Lena: primeiro estado');
rng(0);     %Resultados consistentes
z = rand(512);
p = 0.01;           %Probabilidade de um pixel sofrer ruido S&P
x = z < p;          %Adicionar ruido a matriz x
%garantir que x é uma matriz lógica
MSG= ['x e uma matriz do tipo: ', num2str(class(x)), '.'];
disp(MSG)
fig2 = figure(2);
imshow(x); title('Salt&Pepper');
% Criar uma matriz, v, 3D que adicionará ruido aos 3 canais RGB
v(:, :, 1) = x;         
v(:, :, 2) = x;
v(:, :, 3) = x;
lena(v)=uint8(255);         %Adicionar ruido a imagem 'lena', uint8(255)  em cada canal forca um pixel a branco
fig3 = figure(3);
imshow(lena); title('Salted Lena')
lena(v) = uint8(0);         %adicionar ruido a imagem 'lena', uint8(0) em cada canal forca um pixel a preto
fig4 = figure(4);
imshow(lena); title('Peppered Lena')
% d) Considere a partir daqui que temos uma única realização do processo x[n] com 512×512 valores.
x = x(:);
MSG = ['A estimativa da media do processo e: ', num2str(mean(x)), '. O valor p foi declarado como ' num2str(p), '.'];
disp(MSG)
% e) Verifique que a estimativa da média quadrática do processo é exatamente igual à estimativa da
% média. Explique porquê. Determine a variância do processo<
MSG = ['A estimativa da media quadratica do processo e: ', num2str(mean(x.^2))];
%MSG = ['A media quadratica do processo e: ', num2str(rms(x)), '.'];
disp(MSG)
MSG = ['A estimativa da variancia do processo e: ', num2str(var(x)),'. Que deverá ser aproximado a p * (1 - p) = ', num2str(p * (1 - p)),'.'];
disp(MSG)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 2. Estimativa da autocorrelação
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% a) Estime a autocorrelação r[k] do processo x[n] do ponto anterior, usando a seguinte estimativa
% (polarizada, como iremos ver) do processo x[n].
% Use o comando xcorr, (com MAXLAG=20 e especifique que pretende a estimativa polarizada:
% 'biased').
MAXLAG = 20;
r_hat = xcorr(x, MAXLAG, 'biased');     %Estimativa da autocorrelacao
fig5 = figure(5);
plot(-1 * MAXLAG:MAXLAG, r_hat); grid on; xlabel('k'); ylabel('r_{hat}[k]'); title('Estimativa da autocorrelação do processo x'); xlim([-1 * MAXLAG MAXLAG]);%Como meto um chapeu no r no title?
%Observamos neste plot que a autocorrelação do processo é aproximadamente um impulso de
%Dirac com delta[0] = ~p^2 e um patamar ~p em todos os outros valores
% Estime e determine a potência do processo, E{x.^2[n]} , conhecendo a amostra
% x[n] e sabendo que é um processo de Bernoulli com p conhecido
%r_hat[0] = E{x.^2[n]} = potencia do processo
MSG = ['Estimamos a potencia deste processo: ', num2str(r_hat(21)), '.'];
disp(MSG)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%3. Processo harmónico
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Considere uma realização de um processo harmónico embebido em ruído branco de média nula. Este
% modelo tem muitas aplicações práticas, por exemplo, em telecomunicações ou em análise de
% vibrações.
% Uma amostra de um processo está guardada no seguinte ficheiro: harmonicproc.wav. Leia o ficheiro
% usando o seguinte código
% a) Faça um plot do sinal e ouça-o, de forma a confirmar que existe um tom dominante. Estime a
% frequência do tom usando estes 200 pontos.
% A autocorrelação pode evidenciar a frequência desse tom, que queremos estimar.
[x, fs] = audioread('harmonicproc.wav');
%Pelos vistos devemos usar apenas 200 pontos?
%x = x(1:200);
fig6 = figure(6);
plot(x); title('harmonicproc.wav');grid on; xlabel('n'); ylabel('x[n]'); xlim([0 200]);
%Quero estimar a frequencia do tom dominante, para isso vou estudar a FFT
%https://www.mathworks.com/help/releases/R2019a/matlab/ref/fft.html?container=jshelpbrowser
% X = fft(x);
% L = length(x);
% P2 = abs(X/L);
% P1 = P2(1:L/2+1);
% P1(2:end-1) = 2*P1(2:end-1);
% f = fs*(0:(L/2))/L;
% fig7 = figure(7);
% plot(f,P1); grid on; title('Single-Sided Amplitude Spectrum of X(t)'); xlabel('f (Hz)'); ylabel('|P1(f)|')
% [~, index_max] = max(P1);
% %O vector das frequencias tem 0 Hz no index 1, assim a abcissa/index esta
% %desfasado (adiantado) por 1  em relacao a frequencia. Assim para obtermos
% %a frequncia a partir do indice, subtraimos 1 ao indice
% MSG = ['Temos um valor máximo do espectro de x[n] em: ', num2str(index_max-1),'Hz.'];
% disp(MSG)
% b) Estime a autocorrelação do processo para |k|<=70, isto é, em 141 pontos, de -70:70.
MAXLAG = 70;
r_hat = xcorr(x, MAXLAG, 'biased');
fig8 = figure(8);
plot(-1 * MAXLAG:MAXLAG, r_hat); grid on; xlabel('k'); ylabel('r_{hat}[k]'); title('Autocorrelação do sinal harmonicproc.wav'); xlim([-1 * MAXLAG MAXLAG]);
% c) A partir da estimativa da autocorrelação determine a frequência do processo harmónico (em
% radianos e em Hertz) e a sua amplitude. Estime também a potência (variância) do ruído branco de
% fundo.
%Por analise manual, verificamos que a autocorrelacao tem uma periodicidade
%de 32 amostras (N = 32) assim:
%autocorrelacao dum processo harmonico:
%r_x[k] = (A^2)/2 * cos(w_o * k)
N = 32;                     %Periodo da autocorrelação (em amostras)
w_o = 2 * pi / N;           %Frequencia em radianos
f_hz = fs / N;              %Frequencia em hertz
A = sqrt(2 * r_hat(71-32));    %Amplitude
noise_pow = r_hat(71) - r_hat(71 + 32);      %Potencia do ruido branco
%r_hat[0] é a potencia do sinal + ruido, r_hat[k*N] é a potencia do sinal,
%logo r_hat[0] - r_hat[k*N] = potencia do ruido branco
rr = (A^2/2)*cos(w_o*(-MAXLAG:MAXLAG));         %Estimativa da autocorrelação teorica com parametros determinados na alinea anterior
rr(71) = rr(71) + noise_pow;
fig8;
plot(-MAXLAG:MAXLAG, r_hat, 'b', -MAXLAG:MAXLAG, rr, 'r'); legend('r_{hat}', 'r _{teorico}'); grid on; xlabel('k'); ylabel('r_{hat}[k]'); title('Autocorrelação do sinal harmonicproc.wav'); xlim([-1 * MAXLAG MAXLAG]);
MSG = ['Frequencia da autocorrelacao em radianos: ', num2str(w_o), ' (rad). Em Hz: ', num2str(f_hz), ' (Hz).'];
disp(MSG)









