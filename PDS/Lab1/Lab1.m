%LAB 1 PDS: �udio, Filtragem e DFT
close all; clc; clear; mkdir('Imagens'); delete Imagens/*.*
%Abrir e ler um ficheiro de �udio
[x,fs]=audioread('pcmtest_10Hz.wav'); % l� sinal e freq. de amostragem.
% x: sinal normalizado (entre -1 e 1) numa coluna (mono) ou em 2 colunas (stereo)
%fs: frequ�ncia de amostragem (n� amostras por segundo)
Nc=size(x,2) %n�mero de canais/colunas de x: 1=mono, 2=stereo.
Nx=size(x,1) %n�mero de amostras/linhas de x (comprimento de x[n])
fig = figure
plot(0:Nx-1,x)%gr�fico do sinal (linhas retas entre pontos. Verifique...
%soundsc(x,fs) % reproduz o sinal �udio � frequ�ncia de amostragem fs. Se variar a
title('Sinal �udio original')
grid on
% taxa de amostragem o sinal pode ficar demasiado agudo ou demasiado grave.
print(fig, '-dpng', 'Imagens/sinalaudiooriginal');
%Filtra��o de um sinal com filtro passa-alto 1a ordem, fc = 250hz, gain =
%1, w = pi

fc=250; a=exp(-2*pi*fc/fs); g=(1+a)/2; % exp(-wc*n*T)=an (invar. impulsional)
num = g*[1,-1]; den = [1,-a]; % Polin�mios numerador e denominador de H(z).
[H,w]=freqz(num,den); % H(exp(jw)) em 512 pontos, w=[0,pi[
fig = figure
plot(w/pi,abs(H)) % Plot do m�dulo em freq. normalizada (pi rad corresponde a 1).
title('Filtro passa-alto')
grid on
print(fig, '-dpng', 'Imagens/filtropassalto');
fig = figure
plot(w/pi*fs/2,abs(H)) % Plot em Hz (pois pi rad corresponde a fs/2 Hz).
title('Filtro passa-alto em Hz')
grid on
print(fig, '-dpng', 'Imagens/filtroemHz');
y1=filter(num,den,x);
fig = figure
plot(0:Nx-1,y1)%Verifique que a oscila��o de baixa frequ�ncia desapareceu.
title('Sinal filtrado')
grid on
print(fig, '-dpng', 'Imagens/sinalfiltrado');
%Filtro com forma directa II
y2=zeros(size(x)); %espa�o para sa�da y2[n]; mesmo formato da entrada
w1=0; %condi��o inicial nula.
for n=1:Nx
 w=x(n)+a*w1;
 y2(n)=g*(w-w1);
 w1=w;
end

%vamos comparar y1 com y2
fig = figure;plot(y1-y2), title('Diferenca entre filtro forma I e forma II'), grid on
print(fig, '-dpng', 'Imagens/comparacaodosfiltros');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Parte 2: DFT
z = 0.1 * sin(2*pi*1000/fs*(0:Nx-1)');

Nfft = 512;
k = (0:Nfft/2); %k: 257 �ndices de interesse da DFT. Y0(k+1) s�o os valores da DFT.52
f = k*fs/Nfft; %f: 257 frequ�ncias dos �bins� da DFT, desde f=0 at� f=fs/2.
y = x + z;
Y0=fft(y(1:500),Nfft);
fig = figure
plot(f,abs(Y0(1:257)))
title('DFT do sinal')
grid on
print(fig, '-dpng', 'Imagens/DFTsinal');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Parte 3: Sonograma do sinal
%% Sonograma do sinal com N=500; Nfft=512; M=fs/100
Nfft=512; N=500; M=fs/100;
Nt = floor((Nx-N)/M)+1; %n� de tramas do sinal
Y=zeros(Nfft,Nt); %espa�o para o resultado.
i=1; j=N; %i:j s�o os �ndices da 1� trama.
for m=1:Nt
 trama = y(i:j);
 Y(:,m)= fft(trama,Nfft);
 i=i+M;
 j=j+M;
end
fig = figure;
imagesc(1:Nt,f,db(Y(1:257,:))), axis xy, colorbar, title('Sonograma do sinal')
colormap(jet)
print(fig, '-dpng', 'Imagens/sonograma');
