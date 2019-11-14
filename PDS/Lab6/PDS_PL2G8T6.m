%Lab 6 PDS: Banco de filtros em escala Mel
%PL2
%Grupo 8
%Autor:
%Pedro Silva
close all; clc; clear ; mkdir('Imagens '); delete Imagens/*.*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[x, fs]=audioread('pcmtest.wav');
N = 512; M = 160;
%Gerar tramas de N amostras de M em M amostras
x_frames = buffer(x, N, N-M, 'nodelay');
Nf = floor((length(x) - N)/M) + + 1;        %numero de frames completas
% b) Considere um banco de filtros de 40 canais (Nb=40). Use o código seguinte para calcular a matriz
% H de dimensão 40 x 257 .
Nb=40;N=512;K=N/2; k=0:K; f=k*fs/N;
mu0=0;mu1=1127*log(1+fs/2/700); %mel máxima em fs/2.
melCF=linspace(mu0,mu1,Nb+2); fCF=700*(exp(melCF/1127)-1); %CF: characteristic frequencies
%Cáculo de H e plot dos triângulos:
H=zeros(Nb,K+1);
for i=1:Nb
fa=fCF(i); fc=fCF(i+1); fp=fCF(i+2); %freq. dos vértices do triângulo i.
H(i,:)= (f-fa)/(fc-fa).*(f>=fa & f<=fc) + (f-fp)/(fc-fp).*(f>fc & f<=fp);
plot(fCF,fCF==fCF(i+1)); hold on; title('Frequencias caracteristicas'); xlabel('f(hz)'); ylabel('f_CF');
end
hold off; figure; plot(f,H); title('Matriz H'); xlabel('f(hz'); ylabel('H'); %porquê a diferença?
%Discretizamos o dominio de frequencias [0, fs/2] em 257 passos para o
%banco de filtros H, e esses passos de 8000/257 estão desfasados com o
%vértice dos triângulos das frequencias caracteristicas. 
%FFT das frames
X_frames = fft(x_frames);
% c) Calcule as saídas (energias) para todas as tramas.
%Para o canal i, a energia da m-ésima frame é (y_m[i]) =
%(1/K)sum_k=0^K{H[i,k]*abs(X_m[k])^2. Esta soma é o equivalente do produto
%matricial: y_m = H * abs(X(1:257,:)).^2
y = H * (abs(X_frames(1:257,:)).^2);
% d) Faça um sonograma em dB com o resultado que obteve. Compare este sonograma em escala mel
% com o sonograma DFT. Interprete as diferenças.
% Notas:
% Use a função db() para gerar sonogramas em dB. Use a instrução axis xy e colorbar.
% Não é fácil colocar no sonograma mel as frequências respetivas em Hz, uma vez que a escala não
% é linear. Contudo, podemos alterar as etiquetas verticais do gráfico de acordo com a escala mel
% inversa. Para isso, faça o seguinte:
% 1. gerar o gráfico com a escala mel (usar nº de tramas como abcissa e melCF como ordenada);
% 2. ler os números em mel das etiquetas: umel=get(gca,'YTick')
% 3. alterar para Hz: fmel=700*(exp(umel/1127)-1)'
% 4. passar a strings e colocá-las no gráfico:
% zz=num2str(round(mu)); set(gca,'YTickLabel',cellstr(zz))
Y_db = db(y);
figure(3);
imagesc(1:Nf, f, db(X_frames(1:257,:))), axis xy, colorbar, title('Sonograma de X_m[1:257]')
figure(4);
imagesc(1:Nf, melCF, Y_db), axis xy, colorbar, title('Sonograma de Y_m')


