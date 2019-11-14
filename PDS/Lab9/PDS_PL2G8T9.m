%Lab 8 PDS: Processo Harmónico. Algoritmo MUSIC.
%PL2
%Grupo 8
%Autor:
%Pedro Silva
close all; clc; clear; mkdir('Imagens '); delete Imagens/*.*
%https://en.wikipedia.org/wiki/Periodogram
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1. Autocorrelação do processo
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[x, fs]=audioread('harmonicproc4.wav');
%sound(x,fs)
% b) Calcule a estimativa da autocorrelação (com xcorr), r, a partir das amostras do processo x[n].
% Use MAXLAG=100 pontos de forma a fazer: plot(-100:100,r), grid
MAXLAG = 100;
r_hat = xcorr(x(1:1024), MAXLAG, 'biased');     %Estimativa da autocorrelacao
fig1 = figure(1);
plot(-MAXLAG:MAXLAG, r_hat), grid on; title('Estimativa da autocorrelacao do sinal'); xlabel('n'); ylabel('r\_hat');
%c) Muitas frequencias na autocorrelacao, complicado descobrir a frequencia
%fundamental do sinal e seus harmonicos.
%d)Determinar periodograma com 1024 amostras do sinal
N=1024; NDFT=2*N; %NDFT: um ponto a mais que o necessário
fk = (0:N)/N*fs/2; %N+1 frequências de interesse
X = fft(x(1:N), NDFT); %DFT com 2*N pontos
P = abs(X(1:N+1)).^2/N; %N+1 pontos para k=0:N
fig2 = figure(2);
plot(fk,P), grid on; title('Periodograma do sinal'); xlabel('k'); ylabel('P_N[k]');
r1 = ifft(abs(X).^2/N); %2N pontos, k=0:2N-1
r2 = xcorr(x(1:N),N-1,'biased'); %2N-1 pontos, k=-(N-1):(N-1)
fig3 = figure(3);
plot(-N+1:N-1,r2,-N+1:N,[r1(N+2:end);r1(1:N+1)]) %r1(N+1)=0
grid on; title('Estimativa da autocorrelacao usando IFFT'); xlabel('k'); ylabel('r\_hat[k]'); xlim([-100 100]);
% Aparentemente existem 4 frequências presentes (se colocarmos um limiar de 0.4).
% Estime as frequências e os valores de pico em causa.
[peaks,locs] = findpeaks(P,fk,'MinPeakHeight',0.4); 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%2. Estimativa de pseudoespetro MUSIC
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
R = toeplitz(r_hat(101:140));
[V, D] = eig(R);
fig4 = figure(4);
lambdas = diag(D);      %Extrair VPs da matriz de autocorrelacao
plot(0:39, lambdas, 'o'); grid on; title('Valores proprios da matriz de autocorrelacao R'); xlabel('k'); ylabel('lambda[k]');
%Estimativa da variancia do ruido
disp('Estimamos a variancia do ruido:')
mean(lambdas(1:32))

%%%%%%%PSEUDOESPECTRO USANDO ALGORITMO MUSIC (HAYES)%%%%%%%%%%%%%%%%%%%%%%%
fig5 = figure(5);
PMU = my_music(R, 8);
%que nos da frequencias de harmonicos em:
[peaks2, locs2] = findpeaks(PMU(1:N+1),fk,'MinPeakHeight',0.4)
%O periodograma e a FFT da autocorrelacao. Com o periodograma, estimamos as
%frequencias harmonicas do sinal que suspeitamos existir a partir da
%estimativa da autocorrelacao do sinal




