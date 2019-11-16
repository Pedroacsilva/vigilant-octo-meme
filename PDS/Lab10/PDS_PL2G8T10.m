%Lab 10 PDS: Espetro de Termo Curto, Periodograma e modelo Autorregressivo.
%PL2
%Grupo 8
%Autor:
%Pedro Silva
close all; clc; clear; mkdir('Imagens '); delete Imagens/*.*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 1. Sinal de fala e tramas de 25 ms de 10 em 10 ms
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Leia o ficheiro TVFL_FLI_LIM_F00_896@8.wav e ouça o sinal. Filtre o sinal de forna a eliminar a
% forte componente a 50Hz:
[x, fs] = audioread('TVFL_FLI_LIM_F00_896@8.wav');
h=fir1(500,70*2/fs,'high');
fig1 = figure(1);
freqz(h,1)
print(fig1, '-dpng', 'Imagens/Filtro_HP')
x1=filter(h,1,x);
%comparar:
fig2 = figure(2);
plot(1:2000,x(1:2000),1:2000,x1(251:2250)),grid on; xlabel('n'); ylabel('x'); title('Sinal original e filtrado'); legend('Original', 'Filtrado');
print(fig2, '-dpng', 'Imagens/Sinal_Original_Filtrado')
% Vamos definir uma janela de 25ms, N, que percorre o sinal e que extrai do sinal uma frame.
% Esta janela avança com uma cadencia, M, de 10ms.
M = 80; N = 200;
% Vamos agora definir as tramas do sinal aplicando a janela retangular com estes parâmetros, usando o
% comando buffer:
xf=buffer(x1,N,N-M);
MSG = ['Criamos um buffer com dimensoes ' num2str(size(xf,1)) ' x ' num2str(size(xf,2)), '.'];
disp(MSG)
w=hamming(N); fig3 = figure(3); plot(w); grid on; xlabel('Amostras'); ylabel('Amplitude'); title('Janela de Hamming');
xf=xf.*w; %vetor coluna w é expandido por Nf.
print(fig3, '-dpng', 'Imagens/Janela_Hamming');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 2. Periodograma
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%NDFT => 2 * N_amostras - 1
Nfft=512;
if(Nfft > 2 * size(xf, 1) - 1)
    MSG = ['Tamanho valido para calculo do periodograma. Ndft >= 2 * N_amostras - 1 <==> ', num2str(Nfft), ' >= ', num2str(2 * N - 1 ), '.'];
    disp(MSG)
end
fk = (0:N)/N*fs/2; %N+1 frequências de interesse
%Fazer a FFT de cada frame
%Pre alocar memoria para a FFT de cada frame
Xf = zeros(512, size(xf,2)); P = zeros(512, size(xf,2));
for i = 1:size(xf,2)
    Xf(:, i) = fft(xf(:, i), Nfft); %DFT com 2*N pontos
    P(:, i) = abs(Xf(:, i)).^2/N; %N+1 pontos para k=0:N
end
%Verificar o resultado com um sonograma de P
Nf = size(xf,2); %número de tramas = num de colunas de xfw
f=(0:Nfft/2)/Nfft*fs; %Nfft/2+1=257 valores em [0,fs/2]
fig4 = figure(4);
imagesc(1:Nf,f,db(P(1:Nfft/2+1,:))), axis xy; colorbar; title('Sonograma dos periodograma das frames do sinal')
print(fig4, '-dpng', 'Imagens/Sonograma_Periodograma');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 3. Sonograma LPC
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Vamos assumir que cada trama resulta de um processo autorregressivo, e estimar os parâmetros do
% modelo gerador: filtro só com polos. Vamos tomar 10 polos: p=10.
% Precisamos de resolver as equações de Yule-Walker e para isso precisamos da estimativa da
% autocorrelação de cada trama.
%https://www.youtube.com/watch?v=PFyp4t16_xk
p = 10;
%Estimativas da Autocorrelacao do processo autoregressivo de cada frame
r_hatP = ifft(P);
% A partir da estimativa de autocorrelação rm=rP(:,m) onde m é o índice da trama, defina a matriz
% de autocorrelação Toeplitz de ordem p = 10, R, de dimensão 10x10 (faça help toeplitz). Nesta
% alínea e seguintes use, por exemplo, m=150.
% A partir da estimativa da autocorrelação vamos obter os parâmetros de um modelo autorregressivo
% de ordem p=10 usando o filtro gerador
m = 150;
r_hatm = r_hatP(:,m);
R = toeplitz(r_hatm(1:p));


