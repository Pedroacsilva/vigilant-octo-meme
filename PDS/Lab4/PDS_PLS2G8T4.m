%Lab 4 PDS: Banco de Filtros de 2 Canais de Reconstru��o Perfeita
%PL2
%Grupo 8
%Autores:
%Pedro Carrinho
%Pedro Silva
close all; clc; clear ; mkdir('Imagens ');delete Imagens/*.*
%Filtro FIR Ho(z) fornecido de comprimento N = 36
% a) Copie os coeficientes h0[n] fornecidos (c�digo Matlab) e coloque-os no script. Construa depois
% as respostas a impulso dos filtros 1()Hz, 0 ()Fz e 1()Fz da seguinte forma:
% 1) Defina a vari�vel h0til com coeficientes por ordem inversa dos de h0;
% 2) Construa h1 por troca de sinal dos coeficientes de h0til de ordem par (n=0,2,4,...,N-2);
% 3) Construa f0 e f1 com os valores de h0 e h1 por ordem inversa,
Nh = 36;
h0=[...
0.074088731801707 0.313398921217245 0.596411460394921 0.572232242836783 0.136800277183280 -0.263693327291677,...
-0.188880560852887 0.128670638896525 0.161110885341112 -0.070664426843748 -0.127338395900939 0.044484718467773,...
0.098090567102992 -0.031988886996564 -0.074005520243534 0.025406142155816 0.054360760452705 -0.021225452713612,...
-0.038501762462150 0.017864117805954 0.025969901555583 -0.014690181914019 -0.016410874668589 0.011563575682919,...
0.009473939050820 -0.008573333083088 -0.004770298813289 0.005885767808813 0.001872030114699 -0.003660062664404,...
-0.000326048733191 0.001992796646105 -0.000296972271980 -0.000990559147496 0.000720924535220 -0.000170429382245];

h0til = fliplr(h0);
h1 = h0til;
h1(1:2:end) = -1 * h1(1:2:end);
f0 = fliplr(h0);
f1 = fliplr(h1);

fig1 = figure(1);
subplot(2,2,1)
plot(f0, 'o'); grid on; title('Filtro de s�ntese f_0[n]'); ylabel('f_0[n]'); xlabel('n');
subplot(2,2,2)
plot(f1, 'o'); grid on; title('Filtro de s�ntese f_1[n]'); ylabel('f_1[n]'); xlabel('n');
subplot(2,2,3)
plot(h0, 'o'); grid on; title('Filtro de an�lise h_0[n]'); ylabel('h_0[n]'); xlabel('n');
subplot(2,2,4)
plot(h1, 'o'); grid on; title('Filtro de an�lise h_1[n]'); ylabel('h_1[n]'); xlabel('n');
print(fig1, '-dpng', 'Imagens/filtros_sintese_analise');
% b) Calcule a resposta em frequ�ncia dos 4 filtros usando DFTs de 1024 amostras (em vez de freqz).
% Defina um vetor de frequ�ncias de 513 pontos de forma a fazer um plot do m�dulo das respostas
% em frequ�ncia dos filtros de an�lise, no intervalo [0,?] (frequ�ncia em radianos), [0,1]
% (frequ�ncia normalizada) ou [0, fs/2] (frequ�ncia em Hertz). Use letras mai�sculas para as
% respostas. Verifique que os pares de respostas apresentam simetria de espelho em quadratura
% (QMF).
fig2 = figure(2);
subplot(2,2,1)
frequencias = 0:pi/512:pi;
F0 = fft(f0, 1024);
plot(frequencias, abs(F0(1:end/2+1))); grid on; title('M�dulo da resposta em frequ�ncia de F_0(z)'); ylabel('|F_0(z)|'); xlabel('Frequencia (rads)'); xlim([0 pi])
subplot(2,2,2)
F1 = fft(f1, 1024);
plot(frequencias, abs(F1(1:end/2+1))); grid on; title('M�dulo da resposta em frequ�ncia de F_1(z)'); ylabel('|F_1(z)|'); xlabel('Frequencia (rads)'); xlim([0 pi])
subplot(2,2,3)
H0 = fft(h0, 1024);
plot(frequencias, abs(H0(1:end/2+1))); grid on; title('M�dulo da resposta em frequ�ncia de H_0(z)'); ylabel('|H_0(z)|'); xlabel('Frequencia (rads)'); xlim([0 pi])
subplot(2,2,4)
H1 = fft(h1, 1024);
plot(frequencias, abs(H1(1:end/2+1))); grid on; title('M�dulo da resposta em frequ�ncia de H_1(z)'); ylabel('|H_1(z)|'); xlabel('Frequencia (rads)'); xlim([0 pi])
print(fig2, '-dpng', 'Imagens/f_resp_filtros');

% c) Calcule a resposta em frequ�ncia global do banco de filtros


H = (H0.*F0 + H1.*F1) / 2;
fig3 = figure(3);
subplot(2,1,1)
plot(frequencias, abs(H(1:end/2+1))); grid on; title('M�dulo da resposta em frequ�ncia de H(z)'); ylabel('|H(z)|'); xlabel('Frequencia (rads)'); xlim([0 pi]); ylim([0 2])
subplot(2,1,2)
faseH = unwrap(angle(H))/pi;
plot(frequencias, faseH(1:end/2+1)); grid on; title('Fase da resposta em frequ�ncia de H(z)'); ylabel('arc(H(z))'); xlabel('Frequencia (rads)'); xlim([0 pi]);
print(fig3, '-dpng', 'Imagens/f_transferencia');

% d) Calcule a resposta a impulso do sistema global (por transformada de z inversa da equa��o dada
% na al�nea anterior; use o comando conv). Determine o ganho e atraso do banco de filtros. Para
% verificar que a reconstru��o � perfeita, fa�a um plot desta resposta a impulso e verifique que �
% apenas um impulso. De seguida, fa�a um plot da resposta sem a amostra central (impulso) de
% forma a verificar que os valores obtidos s�o nulos (com 11 algarismos significativos corretos).
%x[n] = delta[n]; H(z)Delta(z) - Z^-1 -> h[n] conv delta[n]

H_glob = 0.5 * (conv(h0, f0) + conv(h1, f1));
fig4 = figure(4);
plot(H_glob, 'o'); grid on; title('Resposta a impulso global por transformada de z inversa'); ylabel('H(z)'); xlabel('j\omega'); xlim([0 length(H_glob)])
print(fig4, '-dpng', 'Imagens/resp_impulso_global')

%Plot da resposta sem amostra central
H_glob_aux = H_glob;
H_glob_aux((length(H_glob) + 1 ) / 2) = [];         %retirar a amostra central
fig5 = figure(5);
plot(H_glob_aux, 'o'); grid on; title('Resposta a impulso global por transformada de z inversa (sem amostra central)'); ylabel('H(z)'); xlabel('j\omega'); xlim([0 length(H_glob)])
print(fig5, '-dpng', 'Imagens/resp_impulso_global_sem_centro')
%uma aproximacao x' tem k algarismos significativos correctos se e s� se
%r(x') < 5 * 10^-k onde r(x') � o desvio entre a nossa aproxima��o e o
%valor real. Portanto, se H_glob tiver valores < 5 * 10^-k, temos k
%algarismos significativos.
disp('Desvio maximo da nossa aproximacao a resposta a impulso do sistema global: ')
max(abs(H_glob))
disp('Temos 12 algarismos significativos correctos')
%Qual o atraso do nosso sistema?
%Atraso do FIR
declive = abs((faseH(end/2+1) - faseH(1)));
atrasoFIR = (Nh - 1) / 2;
%Como as nossas amostram passam por 2 filtros FIR (analise + sintese), o
%atraso total � o dobro do atraso de cada FIR
atrasoTotal = 2 * atrasoFIR;

% e) Determine a autocorrela��o (determin�stica) de h0[n], r[n], que corresponde � fun��o
% 1
% R(z) H0 (z)H0 (z ) ? = . Use o comando r=conv(h0,h0til), mas tome em aten��o que a amostra
% de �ndice 0 se situa a meio do vetor gerado: plot(-(N-1):N-1,r). Verifique que esta
% autocorrela��o � nula nas amostras pares, exceto em n=0, e que, portanto, se trata da solu��o
% paraunit�ria.

r = conv(h0, h0til);
fig6 = figure(6);
plot(-(length(r) - 1) / 2 : (length(r) - 1) / 2, r, 'o'); grid on; title('Autocorrela��o determin�stica de h_0'); xlabel('n'); ylabel('r[n]'); xlim([-(length(r) - 1)/2, (length(r) - 1)/2])
print(fig6, '-dpng', 'Imagens/autocorrelacao')
disp('r[0] = ?')
r((length(r) + 1) / 2)
r_par = r(2:2:end);
r_par((length(r_par)+1)/2) = [];
disp('Max das amostras pares:')
max(abs(r_par))

% 2. Implementa��o do banco de filtros de 2 canais
% b) Use as fun��es para analisar e sintetizar um sinal de �udio com os filtros calculados. Me�a o
% m�ximo erro de reconstru��o e comente.
x = audioread('pcmtest.wav'); % l� sinal e freq. de amostragem.

[v0, v1] = Analysis2(h0, h1, x);
y = Synthesis2(f0, f1, v0, v1);

fig7 = figure(7);
plot(1:length(x), x,1:length(y), y, 'r'); grid on; title('Sinal de entrada e filtrado'); xlabel('n'); ylabel('x[n]'); xlim([0 90]); legend({'Sinal de entrada', 'Sinal filtrado'})
print(fig7, '-dpng', 'Imagens/parte2_sinal_entrada_filtrado')
disp('Maior erro de constru��o')
max(abs(y-x))
disp('Erro muito grande pois o nosso filtro atrasa por 35 amostras')

fig8 = figure(8)
plot(1:length(x)-36, x(1:end-36), 1:length(y)-35, y(36:end), 'r'); grid on; title('Sinais de entrada e de sa�da sobrepostos (atraso compensado)'); xlim([0 90]); legend({'Sinal de entrada', 'Sinal filtrado'})
disp('Maior erro de constru��o tendo em conta o atraso')
max(abs(x(1:end-36) - y(36:end-1)))

% function [v0,v1] = Analysis2(h0,h1,x)
% %[v0,v1] = Analysis2(h0,h1,x)
% %Aplica x[n] ao banco de filtros de 2 canais com respostas a impulso
% %h0 e h1 e decima as sa�das por 2. Retorna estas sa�das, v0 e v1, com metade
% %do comprimento de x. A entrada x deve ser um vetor coluna com comprimento par.
% 
% [n, m] = size(x);
% if m ~= 1
%     error('x deve ser um vector coluna.')
% end
% if mod(n,2) ~= 0
%     error('x deve ter um comprimento par')
% end
% 
% if any(size(h0) ~= size(h1))
%     error('h0 e h1 tem comprimentos diferentes')
% end
% 
% %O numerador � 1 pois o filtro � FIR, e o denominador sao os coeficientes do filtro
% x0 = filter(h0, 1, x);      
% x1 = filter(h1, 1, x);
% %Decimar por 2
% v0 = downsample(x0, 2);         
% v1 = downsample(x1, 2);


% function y = Synthesis2(f0,f1,v0,v1)
% %y = Synthesis2(f0,f1,v0,v1)
% %Expande v0[n] e v1[n] por 2 e aplica-os ao banco de filtros de s�ntese de 2
% %canais com respostas a impulso f0 e f1.
% %As entradas v0 e v1 t�m de ter o mesmo comprimento.
% %Sintetiza a sa�da, y, num vetor coluna de comprimento duplo de v0 e v1.
% 
% if any(size(v0) ~= size(v1))
%     error('v0 e v1 tem comprimentos diferentes')
% end
% 
% %expandir sinais v0 e v1 por 2
% y0 = upsample(v0, 2);
% y1 = upsample(v1, 2);
% 
% %O numerador � 1 pois o filtro � FIR, e o denominador sao os coeficientes do filtro
% y = filter(f0, 1, y0) + filter(f1, 1, y1);

