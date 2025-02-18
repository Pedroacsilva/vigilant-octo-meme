%Lab 13 PDS: Redu��o de Ru�do por Filtragem Wiener na Frequ�ncia - Subtra��o Espetral
%PL2
%Grupo 8
%Autor:
%Pedro Silva
close all; clc; clear; mkdir('Imagens '); delete Imagens/*.*
tic
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 1 - Descri��o
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Neste trabalho vamos experimentar uma t�cnica de redu��o de ru�do baseada em filtragem de Wiener
% no dom�nio da frequ�ncia. A t�cnica de redu��o de ru�do com filtragem temporal implica a
% determina��o de um filtro �timo, causal e est�vel, de acordo com o esquema da figura. A abordagem
% na frequ�ncia permite definir este filtro e a filtragem do sinal no dom�nio da frequ�ncia.
% O sinal de entrada do filtro � x[n] = d[n]+v[n], onde d[n] � o sinal desejado, mas � desconhecido
% porque est� embebido no ru�do v[n]. Apenas x[n] � conhecido. Neste problema � essencial que d[n] n�o esteja correlacionado com o ru�do, o que � verdade mesmo
% em situa��es pr�ticas
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 2 - Trabalho Pr�tico
% 2.1 C�lculo Espetral
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[xx,fs]=audioread('PT3.wav');
Nx = size(xx,1); %dura��o do sinal em amostras
ncanais = size(xx,2); %dois canais
%ouvir o sinal
%soundsc(xx, fs)
x = xx(:,1); %canal esquerdo; um canal de cada vez; noutro passo tomar xx(:,2).
N = fix(fs*0.03); %tamanho das tramas
M = fix(fs*0.01); %avan�o da janela de an�lise
Ntramas = fix((Nx-N)/M)+1; %n� de tramas de N amostras com avan�o de M
jan = hamming(N); %janela de Hamming (uma coluna)
dcsomajan = sum(jan)/M; %fator ou ganho da soma com sobreposi��o das janelas.
alfa = 0.01; %-40dB
Nfft = 2^nextpow2(N); %tamanho das DFTs: 2048
X = zeros(Nfft,Ntramas); %espa�o para as DFTs, uma por cada coluna
E = zeros(1,Ntramas); %espa�o para a energia das tramas
i=1; %in�cio da 1a trama
j=N; %fim da 1a trama
for m=1:Ntramas
    trama = x(i:j).*jan; %a trama m
    Xtrama = fft(trama,Nfft); %a DFT da trama m com Nfft pontos.
    X(:,m) = Xtrama;% guardar a DFT em vez do periodograma.
    E(m) = sqrt(sum(trama.^2)); % raiz quadrada da energia da trama m
    i=i+M; %avan�ar M amostras para a pr�xima trama.
    j=j+M; %avan�ar M amostras para a pr�xima trama.
end
% mostrar o sonograma do sinal, s� acima de alfa dBs:
f=(0:Nfft/2)/Nfft*fs; %vetor de frequ�ncias em Hz para o sonograma
fig1 = figure(1); imagesc(1:Ntramas,f,max(db(X(1:Nfft/2+1,:)),db(alfa))), colorbar, axis xy; xlabel('Trama'); ylabel('Frequencia'); title('Sonograma do sinal PT3.wav');
print(fig1, '-dpng', 'Imagens/Sonograma_PT3');
fig2 = figure(2); plot(1:Ntramas,db(E)), grid on, xlabel('Trama'); ylabel('Energia'); title('Energia das tramas do sinal PT3.wav'); %energia em dB (ou 10*log10(E^2))
print(fig2, '-dpng', 'Imagens/Energia_tramas_PT3');
disp('Pelo sonograma, observamos que temos 2 tons constantes a 10kHz e 8kHz. Observamos tamb�m uma barra constante a 50Hz (ru�do) e outra barra, menos intensa, a 150Hz (um potencial harm�nico do ru�do)')
disp('Pela energia do sinal, estimamos um valor de threshold para a diferencia��o de ru�do-sinal de -10 dB, ie: abaixo de -10 dB consideramos ru�do, acima consideramos sinal');
limiar = -10;
i=find(db(E)<limiar);       %Obter as tramas consideradas como ru�do
Xv_est = mean(abs(X(:,i)),2);
fig3 = figure(3);
print(fig3, '-dpng', 'Imagens/Energia_tramas_ruidosas');
plot(f,db(Xv_est(1:Nfft/2+1))),grid; xlabel('Frequencia'); ylabel('Energia'); title('Energia das tramas ruidosas');
% 2.3 Subtra��o Espetral:
% A estimativa do ru�do � tomada assumindo que o ru�do se mant�m ao longo deste sinal. Da� que temos
% apenas um valor para cada frequ�ncia. 

Xx = abs(X); %nos Nfft valores (duplicado de pi a 2*pi: plot(Xx))
Xd_est = max( Xx - Xv_est , alfa*Xx); %subtra��o espetral (n�o linear): Eq. 1
fig4 = figure(4); imagesc(1:Ntramas,f,db(max(Xd_est(1:Nfft/2+1,:),alfa))), axis xy; xlabel('Tramas'); ylabel('Frequencia'); title('Sonograma do sinal PT3.wav ap�s subtraccao espectral');
print(fig4, '-dpng', 'Imagens/Sonograma_Pos_SubEspectral');
colorbar

% 2.4 Filtro de Wiener
% Calcule o filtro de Wiener (equa��o (2)) usando Xd_est e Xv_est em vez dos espetros de pot�ncia
%filtro de Wiener:
H = Xd_est.^2./(Xd_est.^2 + Xv_est.^2); %com |X|^2; n�o � necess�rio dividir por N
size(H) %deve ter a mesma dimens�o de X
isreal(H) %� real?
Y = H.*X; % espetro da sa�da (sinal melhorado)
fig5 = figure(5); imagesc(1:Ntramas,f,max(db(X(1:Nfft/2+1,:)),-60)), axis xy, colorbar, ylabel('Frequencia'); xlabel('Trama'); title('Sonograma original');
fig6 = figure(6); imagesc(1:Ntramas,f,max(db(Y(1:Nfft/2+1,:)),-60)), axis xy, colorbar, xlabel('Trama'); ylabel('Frequencia'); title('Sonograma do sinal filtrado por H(e^{jw})')
print(fig6, '-dpng', 'Imagens/Sonograma_Sinal_Melhorado');
disp('Comparando os dois sonogramas, o sonograma do sinal filtrado tem menor ruido de fundo em todas as tramas (cor de fundo � um azul mais escuro). O original tem um fundo dum azul mais claro, indicando mais ruido.');
disp('Observamos tambem que as duas faixas de 50 e 150Hz, indicativas do ruido, foram removidas.'); 
disp('No entanto, TODO O SINAL, nessas frequencias, foi removido, portanto poderemos melhorar a nossa abordagem.');


% 2.5 S�ntese do sinal
% Fa�a a IDFT das tramas Y e divida pela m�dia da soma das janelas:
y = real(ifft(Y))/dcsomajan; %tramas para OLA (overlap-add)
size(y) %Nfft valores por trama. Tomar apenas os 1�s N valores de cada trama.
fig7 = figure(7); plot(y(:,140)), xlabel('n'); title('Trama 140 do sinal filtrado pelo filtro de Wiener ap�s IFFT'); %Exemplo.
print(fig7, '-dpng', 'Imagens/Trama_140_Sinal_Melhorado_posiff');
disp('De notar que como modificamos o espectro do sinal, teremos uma linha ruidosa em y = 0, quando, se estudarmos a trama 140 de x, temos uma linha costante a y = 0 para essas amostras');
%% overlap-add
yy=zeros(N+(Ntramas-1)*M,1); %espa�o suficiente para o resultado
i=1; j=N;
for m=1:Ntramas
yy(i:j)=yy(i:j)+y(1:N,m); %soma com sobreposi��o de N-M=882 amostras.
i= i + M;
j= j + M;
end
disp('Sintetisado com metodo OLA');
soundsc(yy, fs)
pause(10)
fig8 = figure(8); plot(yy), grid on,  title('Sinal sintetisado com o m�todo overlap-add');
print(fig8, '-dpng', 'Imagens/Sinal_filtrado_metodo_OLA');
%A partir desta figura diga se o sinal foi ou n�o melhorado.

disp('Observamos, nos momentos silenciosos do sinal, que o seu tra�ado tem valores de valor absoluto muito pequenos (~< 10^-3)');

%%Pode experimentar o resultado da soma das janelas com sobreposi��o
% fazendo no ciclo anterior y=jan (equivale a fazer x[n]=1):
yy=zeros(N+(Ntramas-1)*M,1); %espa�o suficiente para o resultado
i=1; j=N;
for m=1:Ntramas
    yy(i:j)= yy(i:j)+jan; %soma com sobreposi��o de N-M=882 amostras.
    i= i + M;
    j= j + M;
end
fig9 = figure(9); plot(yy), grid on, title('Soma das janelas das tramas com sobreposi��o');
print(fig9, '-dpng', 'Imagens/Soma_Janelas_Tramas');
disp('Fazendo zoom, reparamos que o ripple se repete a cada M = 441 amostras, com um valor m�dio de dcsomajan = 1.6190');
toc