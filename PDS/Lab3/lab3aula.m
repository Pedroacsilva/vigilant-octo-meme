%Lab 3 PDS: Conversao de Frequencia de amostragem
%PL2
%Grupo 8
%Autores:
%Pedro Ribeiro
%Pedro Silva
close all; clc; clear ; mkdir('Imagens ');delete Imagens/*.*
%a) Considere um sinal de fala com frequência de amostragem de fs=16 kHz, por exemplo
%'pcmtest.wav'. Identifique os fatores de expansão, L, e de decimação, M.
[x,fs_in] = audioread('pcmtest.wav'); % lê sinal e freq. de amostragem.
fs_out = 44100;     %frequencia de entrada é 16kHz, e queremos que a frequencia de saida seja 44100Hz.
%[L,M] = numden(sym(fs_out/fs_in);
[L, M] = rat(44100/16000);            %https://www.mathworks.com/help/releases/R2019a/matlab/ref/rats.html?container=jshelpbrowser
%^-- descobrir os factores de expansão e de decimação (L, M).
%L > M, porque queremos aumentar a frequencia do sinal
% b) Projete um filtro FIR, passa-baixo, com comprimento Nh=3529 (ordem 3528) com o método das
% janelas (comando fir1). Aplique um ganho L para que as amplitudes de entrada e de saída sejam
% iguais.
Nh = 3529;      %Comprimento do filtro FIR
h = fir1(Nh-1, 1/L) * L;        %nao esquecer compensar o ganho do filtro
[H,w] = freqz(h, 1, Nh);      %resposta em frequencia do filtro 
fig1 = figure(1);
semilogx(w/pi, abs(H))
grid on; title('Filtro H(z) passa baixo para w_c = 1/L'); xlabel('Frequencia normalizada'); ylabel('|H(z)|')
print(fig1, '-dpng', 'Imagens/ganho_H_z')
fig2 = figure(2);
plot(h)
grid on; title('Filtro h(n) passa baixo para w_c = 1/L');
print(fig2, '-dpng', 'Imagens/filtro_h_n')
% c) Faça a implementação direta do sistema para 16000 amostras (1 segundo do sinal de entrada,
% mono), em analogia com o trabalho prático anterior. Guarde estas amostras na variável y1 (para
% conferir depois o resultado com o método eficiente). Confirme que guardou 44100 amostras de
% saída, isto é, 1 segundo de sinal de saída.
x_in = x(1:16000);          %um segundo do sinal de entrada
xe = upsample(x_in, L);     %Sinal expandido
xh = filter(h, 1, xe);          %sinal interpolado (saida do filtro H(z)
y1 = xh(1:M:end);           %sinal decimado. Saida do filtro decimada por M
%gráfico de Nx1 amostras de entrada e Ny1 amostras de saída:
Nx1=30; Nh1=Nx1*L; Ny1=ceil(Nh1/M);
fig3 = figure(3);
plot(4*L+1:L:Nh1,x(1:Nx1-4),'*',1:Nh1,xh(1:Nh1),1:M:Ny1*M,y1(1:Ny1),'o')
grid on; ; title('Um segundo do sinal de entrada, interpolado, e de saida'); xlabel('Tempo (s)'); ylabel('Sinal'); legend({'x[n]','xh[n]','y[n]'})
print(fig3, '-dpng', 'Imagens/sinais_entrada_interpolado_saida')
% d) Faça agora uma implementação eficiente, calculando a saída no vetor y2. (aloque espaço
% inicialmente para y2). Faça um ciclo para calcular cada valor de y[n], tomando como primeiro
% índice k o resto da divisão de nM por L. Pare o ciclo se k ultrapassar o valor Nh?1 ou se o índice
% do sinal de entrada for negativo.
% Sugestão: use índices a começar em zero e indexe as variáveis do Matlab com avanço de 1, por
% exemplo, h(k+1) para se referir a h[k].
% No final, compare as amostras de y2 com y1. Nota: faça um gráfico do erro relativo a y1; (os
% valores devem conferir em 12 ou mais algarismos significativos). Verifique primeiro se as
% dimensões de y1 e de y2 coincidem.

y2 = zeros(44100, 1);           %Alocar memória para o matlab
for n = 0:44100-1       %calcular 44100 sinais de saida
    nM = n * M;
    k = mod(nM, L);
    m = floor((nM - k) / L);
    %k = nM - m * L;
    sum = 0;            %inicializar somatorio
    while k < Nh && m >= 0      %condicoes do somatorio
        sum = sum + (h(k+1) * x(m+1));
        k = k + L;
        m = m - 1;
    end
    %Este ciclo while corre (Nh-1 / L) vezes. Neste caso, 8 vezes
    y2(n+1) = sum;
end
fig4 = figure(4);
subplot(3,1,1)
plot(1:44100, y1, 'r')
grid on; title('Filtro ineficiente'); xlabel('Amostra (n)'); ylabel('y1[n]');
subplot(3,1,2)
plot(1:44100, y2, 'r')
grid on; title('Filtro eficiente'); xlabel('Amostra (n)'); ylabel('y2[n]');
subplot(3,1,3)
erro = abs(y1-y2);
plot(1:44100, erro, 'm')
grid on; title('Diferença entre filtros'); xlabel('Amostra (n)'); ylabel('|y1[n] - y[2]|');
print(fig4, '-dpng', 'Imagens/comparacao_filtros')
% f) Repita o cálculo usando agora o comando upfirdn(), calculando a saída na variável y3. Compare
% as primeiras 44100 amostras da saída y3 com y2: devem ser exatamente iguais. Porquê?
% Use: max(abs(y2-y3(1:44100)))
y3 = upfirdn(x, h, L, M);
max(abs(y2-y3(1:44100)))
% h) Usando o algoritmo eficiente (ou upfirdn()) calcule a saída relativa ao sinal x[n] completo.
% Depois ouça os dois sinais às frequências respetivas. Nota alguma diferença?
fig5 = figure(5)
subplot(2,1,1)
plot(x)
grid on; title('Sinal de entrada'); xlabel('n'); ylabel('x[n]')
subplot(2,1,2)
plot(y3,'r')
grid on; title('Sinal interpolado com upfirdn'); xlabel('n'); ylabel('y_3[n]')
print(fig5, '-dpng', 'Imagens/comparacao_entrada_upfirdn')
disp('Sinal de entrada')
sound(x, 16000)
disp('Sinal interpolado eficientemente')
pause(3)
sound(y3, 44100)




        













