%Lab 12 PDS: Filtros óptimos- cancelamento de ruído
%PL2
%Grupo 8
%Autor:
%Pedro Silva
close all; clc; clear; mkdir('Imagens '); delete Imagens/*.*
tic
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 1. Obtenção do filtro de ordem p=10.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

[d, fs_d] = audioread('signal_mike3.wav');
disp('Sinal d')
soundsc(d,fs_d)
pause(4)
[x, fs_x] = audioread('noise_mike3.wav');
disp('Sinal x')
soundsc(x, fs_x)
pause(4)
%Filtro H(z) que estima v1, tal que o erro, e, tenha a minima potencia
%possivel. d = s + v1, e = d - y. x filtrado por H(z) = y.
%H(z) é de ordem p = 10.
p = 10;
% a) Estime rx[k] e rdx[k] para p+1 pontos, e construa a matriz Toeplitz Rx.
r_x = xcorr(x, p, 'biased');
r_dx = xcorr(d, x, p, 'biased');
fig1 = figure(1);
subplot(2,1,1)
plot(-p:p, r_x); grid on; xlabel('n'); ylabel('r_x[n]'); title('Autocorrelação de x[n]');
subplot(2,1,2)
plot(-p:p, r_dx); grid on; xlabel('n'); ylabel('r_{dx}[n]'); title('Correlação de d[n] com x[n]');
print(fig1, '-dpng', 'Imagens/r_x__r_dx');
R_x = toeplitz(r_x(p+1:end));

% b) Determine os coeficientes do filtro, h, através das equações de
% Wiener-Hopf: R_x h = r_dx
h =  R_x \ r_dx(p+1:end);
fig2 = figure(2);
plot(h, 'o'), grid on; xlabel('n'); ylabel('h[n]'); title('Coeficientes do filtro de Wiener H(z)')
print(fig2, '-dpng', 'Imagens/Coef_h_z')
% c) Calcule y[n] aplicando o sinal x[n] ao filtro obtido (use o comando filter()). Com o sinal d[n]
% calcule e[n].
% Faça um plot deste sinal e verifique que já é possível perceber que existe uma mensagem presente no
% sinal.
y = filter(h, 1, x);            %Ganho de H(z) = 1?
e = d - y;
disp('Sinal e, d - x')
soundsc(e, fs_x)
fig3 = figure(3);
plot(e), grid on; xlim([0, length(e)]); xlabel('n'); ylabel('e[n]'); title('Sinal e[n]. Fala - Ruido');
print(fig3, '-dpng', 'Imagens/Sinal_e');
% d) Verifique que é possível fazer melhor, usando um filtro mais longo. Repita o procedimento anterior
% para p=20 e depois p=50.
for p = 20:30:50
    MSG = ['Repetir procedimento com p = ', num2str(p), '.'];
    disp(MSG)
    r_x = xcorr(x, p, 'biased');
    r_dx = xcorr(d, x, p, 'biased');
    R_x = toeplitz(r_x(p+1:end));
    h =  R_x \ r_dx(p+1:end);
    y = filter(h, 1, x);
    e = d - y;
    soundsc(e, fs_x)
    pause(4)
end
disp('Um filtro maior (mais coeficientes) reduz mais a potencia do sinal de erro, e = d - x, levando a menor ruido no sinal.')


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 2. Verificação dos filtros
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% a) Verifique que x[n] é um processo autorregressivo de ordem 6 com filtro
% gerador H(z) = 0.05 / A_x(z), onde A_x(z)=
% ax=[1,-1.54,2.36,-2.25,2.13,-1.25,0.74]
ax_matlab = aryule(x, 6)
%Nao usando ferramentas nativas ao matlab...
R_x = toeplitz(r_x(p+1:p+6));
a_x = R_x \ (-1 .* r_x(p+2:p+7));
a_x = [1, a_x']
disp('ax=[1,-1.54,2.36,-2.25,2.13,-1.25,0.74]')

% Faça um esquema em que um mesmo sinal ruído branco, por exemplo v[n], é a entrada comum dos
% filtros H_1(z) e H_x(z). Nesta situação, considerando s[n]=0, determine a solução analítica para o filtro
% ótimo H_opt(z). Verifique que é IIR.
% Compare esta solução (que é IIR) com a solução FIR obtida numericamente (filtro ótimo de Wiener).
% Compare as respetivas respostas a impulso e respostas em frequência.
% Para gerar um segmento da resposta a impulso real, considere o seguinte sinal (impulso):
% imp=[1,zeros(1,p)]; aplique este sinal ao filtro IIR H_opt(z) e determine a saída (resposta a impulso)
% com o comando filter().

%Filtro optimo: H_x(z) * H(z) = H_opt(z). Neste caso, os dois ramos, com
%v[n] comum a entrada, sao iguais, consequentemente a diferenca entre eles
%e nula (idealmente) e[n] = 0, para qualquer n 

b_1 = [1 -0.2 -0.8];
gain = 0.15;
B_1 = gain .* b_1;
a_1 = [1 -0.6 -0.27];

num = conv(a_x, B_1);
den = 0.05 * a_1;
[H, w] = freqz(h, 1);
H_opt = freqz(num, den);
fig4 = figure(4);
plot(w, abs(H), w, abs(H_opt)); grid on; xlabel('z'); ylabel('H(z)'); title('Resposta em frequencia dos filtros H e H_{optimo}'); legend('H(z)', 'H_{optimo}(z)'); xlim([0 pi])
print(fig4, '-dpng', 'Imagens/Resp_Freq_H_H_optimo')
impulso = [1, zeros(1,p)];
h_opt = filter(num, den, impulso);
h_wiener = filter(h, 1, impulso);
fig5 = figure(5);
frequencias = linspace(-pi, pi, size(h_opt,2));
fig5 = figure(5);
plot(frequencias, h_wiener, frequencias, h_opt);  grid on; xlabel('z'); ylabel('H(z)'); title('Resposta a impulso dos filtros H e H_{optimo}'); legend('H(z)', 'H_{optimo}(z)'); xlim([-pi pi])
print(fig5, '-dpng', 'Imagens/Resp_Imp_H_H_optimo')
MSG = ['Erro maximo entre h_opt e h_wiener: ', num2str(max(abs(h_opt - h_wiener))), '.'];
disp(MSG)
MSG = ['Erro maximo entre H_optimo e H_wiener: ', num2str(max(abs(H_opt - H))), '.'];
disp(MSG)
