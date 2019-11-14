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
% b) Considere um banco de filtros de 40 canais (Nb=40). Use o c�digo seguinte para calcular a matriz
% H de dimens�o 40 x 257 .
Nb=40;N=512;K=N/2; k=0:K; f=k*fs/N;
mu0=0;mu1=1127*log(1+fs/2/700); %mel m�xima em fs/2.
melCF=linspace(mu0,mu1,Nb+2); fCF=700*(exp(melCF/1127)-1); %CF: characteristic frequencies
%C�culo de H e plot dos tri�ngulos:
H=zeros(Nb,K+1);
for i=1:Nb
fa=fCF(i); fc=fCF(i+1); fp=fCF(i+2); %freq. dos v�rtices do tri�ngulo i.
H(i,:)= (f-fa)/(fc-fa).*(f>=fa & f<=fc) + (f-fp)/(fc-fp).*(f>fc & f<=fp);
plot(fCF,fCF==fCF(i+1)); hold on; title('Frequencias caracteristicas'); xlabel('f(hz)'); ylabel('f_CF');
end
hold off; figure; plot(f,H); title('Matriz H'); xlabel('f(hz'); ylabel('H'); %porqu� a diferen�a?
%Discretizamos o dominio de frequencias [0, fs/2] em 257 passos para o
%banco de filtros H, e esses passos de 8000/257 est�o desfasados com o
%v�rtice dos tri�ngulos das frequencias caracteristicas. 
%FFT das frames
X_frames = fft(x_frames);
% c) Calcule as sa�das (energias) para todas as tramas.
%Para o canal i, a energia da m-�sima frame � (y_m[i]) =
%(1/K)sum_k=0^K{H[i,k]*abs(X_m[k])^2. Esta soma � o equivalente do produto
%matricial: y_m = H * abs(X(1:257,:)).^2
y = H * (abs(X_frames(1:257,:)).^2);
% d) Fa�a um sonograma em dB com o resultado que obteve. Compare este sonograma em escala mel
% com o sonograma DFT. Interprete as diferen�as.
% Notas:
% Use a fun��o db() para gerar sonogramas em dB. Use a instru��o axis xy e colorbar.
% N�o � f�cil colocar no sonograma mel as frequ�ncias respetivas em Hz, uma vez que a escala n�o
% � linear. Contudo, podemos alterar as etiquetas verticais do gr�fico de acordo com a escala mel
% inversa. Para isso, fa�a o seguinte:
% 1. gerar o gr�fico com a escala mel (usar n� de tramas como abcissa e melCF como ordenada);
% 2. ler os n�meros em mel das etiquetas: umel=get(gca,'YTick')
% 3. alterar para Hz: fmel=700*(exp(umel/1127)-1)'
% 4. passar a strings e coloc�-las no gr�fico:
% zz=num2str(round(mu)); set(gca,'YTickLabel',cellstr(zz))
Y_db = db(y);
figure(3);
imagesc(1:Nf, f, db(X_frames(1:257,:))), axis xy, colorbar, title('Sonograma de X_m[1:257]')
figure(4);
imagesc(1:Nf, melCF, Y_db), axis xy, colorbar, title('Sonograma de Y_m')


