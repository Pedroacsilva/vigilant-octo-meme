%Lab 11 PDS: Estimação Espetral de Potência
%PL2
%Grupo 8
%Autor:
%Pedro Silva
close all; clc; clear; mkdir('Imagens '); delete Imagens/*.*
tic
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 1. Periodograma
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
f=fopen('temperaturas.dat','rb');
temp = fread(f, inf, 'float32');
fclose(f);
fig1 = figure(1);
plot(temp); grid on; xlabel('tempo'); ylabel('temperatura'); title('Temperaturas de um escritório, 2 amostras por hora');
print(fig1, '-dpng', 'Imagens/plot_temperaturas_bihoral');
fig2 = figure(2);
plot((0:length(temp)-1)/48, temp);grid on; xlabel('tempo'); ylabel('temperatura'); title('Temperaturas de um escritório, amostras diarias');
print(fig2, '-dpng', 'Imagens/plot_temperaturas_diarias')
fig3 = figure(3);
plot((0:length(temp)-1)/(48*7), temp);grid on; xlabel('tempo'); ylabel('temperatura'); title('Temperaturas de um escritório, amostras semanais');
print(fig2, '-dpng', 'Imagens/plot_temperaturas_semanais')

fs_dia = 48;            %ciclos por dia
fs_semana = 7 * 48;     %ciclos por semana
NFFT = 8192;
N = length(temp);
fk = (0:NFFT/2)/NFFT * fs_dia;       %[0, fs_dia/2];
P = abs(fft(temp - mean(temp), NFFT)).^2/N;
fig4 = figure(4);
plot(fk, P(1:NFFT/2+1)), grid on; xlabel('Ciclos por dia'); ylabel('P[k]'); title('Periodograma. Frequencia de amostragem diaria')
print(fig4, '-dpng', 'Imagens/Periodograma_freq_diaria')
fig5 = figure(5);
fk_semana = (0:NFFT/2)/NFFT * fs_semana;       %[0, fs_semana/2];
plot(fk_semana, P(1:NFFT/2+1)), grid on; xlabel('Ciclos por semana'); ylabel('P[k]'); title('Periodograma. Frequencia de amostragem semanal')
disp('Os tres picos nos periodogramas dizem-nos que existe uma variancia forte com uma cadencia de 1 vez por semana, de 2 vezes por semana, e de 7 vezes por semana. Trata-se de aquecer o escritorio diariamente, apos um fim-de-semana, e nos fins de semanas(provavelmente).')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 2. Polarização do periodograma
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Vamos considerar um processo AR, dum filtro gerador com ganho unitario e
%polos em p1,2 = 0.98*exp(+-j10pi/50) e p3,4 = 0.98*exp(+-j14pi/50);
fs=10000; Nfft=8192;
wn=(0:Nfft/2)/Nfft*2; w1=pi/5; w2=14*pi/50;
p12=0.98*[cos(w1)+1i*sin(w1), cos(w1)-1i*sin(w1)];
p34=0.98*[cos(w2)+1i*sin(w2), cos(w2)-1i*sin(w2)];
A=poly([p12,p34]); %poly() é o inverso de roots(). A tem coef. reais.
H=1./fft(A,Nfft); % H[k]=N[k]/D[k]
Ptrue=abs(H).^2; %com 8192 pontos em [0,2*pi[
k=1:Nfft/2+1; %os 4097 índices de interesse para w em [0,pi]
fig6 = figure(6);
plot(wn,db(Ptrue(k))/2), grid on; xlabel('w_n'); ylabel('P_{true}(e^{jw}'); title('Verdadeiro espectro de potencia do processo AR');


rng(0);
N = 1000;
v = randn(1, N);        %Amostra de ruido branco de 1000 amostras.
x = filter(1, A, v);        %Saida do filtro H(Z), x[n], com entrada de ruido branco, v[n]
P = abs(fft(x, NFFT)).^2 /N;
fig7 = figure(7);
plot(wn, db(P(k))/2, wn, db(Ptrue(k))/2), grid on; xlabel('w_n'); ylabel('P(e^{jw}'); title('Estimativa do espectro de potencia'); legend('P_{true}', 'P_{hat}');
%Queremos agora sobrepor o verdadeiro espectro de potencia, a sua
%estimativa, e o valor esperado do periodograma com a ifft de H(e^jw)
rtrue = ifft(Ptrue);
WR=fft(ones(1,N),Nfft);
wb = abs(WR).^2 /N;
fig8 = figure(8);
plot(wb), grid on; xlabel('Amostras'); ylabel('Amplitude'); title('Espectro da janela triangular')
fig9 = figure(9);
wBartlett = ifft(wb);
plot(wBartlett), grid on; xlabel('Amostras'); ylabel('Amplitude'); title('Janela Triangular')
EP = fft(rtrue .* wBartlett);
EP = real(EP);      %Devido a erros de aproximacao, tinhamos partes imaginarias
fig10 = figure(10);
plot(wn, db(P(k))/2, wn, db(Ptrue(k))/2, wn, db(EP(k))/2); grid on;  xlabel('w_n'); ylabel('P(e^{jw}'); title('P_{true}, Estimativa do periodograma, e esperanca do periodograma'); legend({'P_{true}', 'P_{hat}', 'EP'});
print(fig10, '-dpng', 'Imagens/Ptrue_Phat_EP')
disp('Observamos uma polarização a altas frequencias! A nossa estimativa apresenta erros a HF.')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 3. Método de Welch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Considere ainda o processo AR(4) do ponto anterior, mas agora gere uma amostra com 30000 valores
% do processo.
N = 30000;
v = randn(1, N);        %Amostra de ruido branco de 30000 amostras.
x = filter(1, A, v);
fig11 = figure(11);
plot(db(abs(fft(x,32 * 1024)).^2 / N)), grid on; xlabel('w_n'); ylabel('P(e^{jw}'); title('Periodograma com 30000 amostras');
print(fig11, '-dpng', 'Imagens/Periodograma_30000')
disp('O aumento de amostras não diminui a variancia do periodograma!!')
% b) Use agora o método de Welch para gerar uma estimativa consistente. Para isso, use secções de
% 1000 amostras com a janela de Hamming com avanço de 200 amostras (sobreposição de 800
% amostras) e DFTs de comprimento 2048

N=1000; Nfft=2048;
hw = hamming(N); U=sum(hw.^2)/N;
xf=buffer(x,N,800).*hw; %tramas com janela de Hamming
Nf=size(xf,2); %150 tramas (frames)
Pm = abs(fft(xf,Nfft)).^2/(N*U); %150 periodogramas modificados
%Verificar que Pm é uma matriz (real) de dimensão 2048x150
%É necessário fazer a média na 2ª dimensão => vetor coluna de 2048x1
%Para fazer plot conjunto de Pwelch com Ptrue, defina:
Pwelch = mean(Pm,2);
wn2 = (0:1024)/1024;
fig12 = figure(12);
plot(wn2,db(Pwelch(1:1025))/2,wn,db(Ptrue(k))/2), grid on; xlabel('x_n'); ylabel('P(e^{jw})'); title('Periodograma e média de 150 Periodogramas'); legend('P_{Welch}', 'P_{true}');
disp('Observamos que o periodograma com o método de Welch é uma boa estimativa para o espectro de potencia do processo AR!')

toc