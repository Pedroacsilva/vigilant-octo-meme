%Lab 7 PDS: Variáveis aleatórias, histogramas e normalização de amostras.
%PL2
%Grupo 8
%Autor:
%Pedro Silva
% Neste trabalho pretende-se elaborar os conceitos de variável aleatória (VA), amostra, função
% densidade de probabilidade e normalização de variáveis aleatórias. Para isso, iremos usar os dois
% geradores de números pseudoaleatórios do Matlab: rand (distribuição uniforme entre 0 e 1) e randn
% (distribuição normal ou Gaussiana, de média zero e variância 1). Os números gerados são
% pseudoaleatórios pois podemos repetir a amostra inicializando o gerador no mesmo estado
% (rng(seed)).
close all; clc; clear; mkdir('Imagens '); delete Imagens/*.*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 1. Amostra 1D com distribuição triangular.
rng(0) %para o caso de querermos repetir a experiência.
x1 = rand(1,1e5); %uma amostra com 10^5 realizações
x2 = rand(1,1e5); %outra: independente e identicamente distribuída.
x_tri = x1+x2;
fig1 = figure(1);
histogram(x_tri, 100, 'Normalization', 'pdf'); title('Histograma de X (distribuição triangular)');
print(fig1, '-dpng', 'Imagens/histograma_X')
MSG = ['Estimativa da média de x: ', num2str(mean(x_tri)),'. Estimativa da variância de x: ', num2str(var(x_tri)),'.'];
disp(MSG)
%Matematicamente, sabemos que a média de X_tri é 1 e a variância é 1/6
fig2 = figure(2);
subplot(2,1,1)
histogram(x1, 100, 'Normalization', 'pdf'); title('Histograma da VA x1');
subplot(2,1,2)
histogram(x2, 100, 'Normalization', 'pdf'); title('Histograma da VA x2');
print(fig2, '-dpng', 'Imagens/histogramas_x1_x2')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 2. Amostra 1D com distribuição Gaussiana.
rng(0) %para o caso de querermos repetir a experiência.
x_gauss = 7*randn(1,100000)+3; %amostra com 100k pontos
%z segue uma distribuição gaussiana com esperança 0 e variancia 1.
%Assim, x = z + 3 tem esperança 3 e variancia=
%E{(x-3)^2}=E{(7z)^2}=7^2*E{z}=49. 
%A nossa distribuição tem esperança 3 e variancia 49.
fig3 = figure(3);
h = histogram(x_gauss, 100, 'Normalization', 'pdf');
mean_x_gauss = mean(x_gauss);
var_x_gauss = var(x_gauss);
MSG = ['Estimativa da média de x gauss: ', num2str(mean_x_gauss),'. Estimativa da variância de x gauss: ', num2str(var_x_gauss),'.'];
disp(MSG)
xpdf = linspace(-30, 40, 201);
fpdf = exp(-((xpdf-3).^2)/(2*49))/sqrt(2*pi*49);
hold on; grid on;
plot(xpdf, fpdf); title('Histograma de X (dist. Gaussiana) e sua Probability Density Function')
legend('Histograma', 'PDF')
print(fig3, '-dpng', 'Imagens/histogramas_XGauss_PDF')
%Vemos assim que a probability density function segue o histograma
%normalizado.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 3. Vetor de duas variáveis aleatórias
means_y  = [27; 7];
covar_y = [4 5; 5 9];
means_x = [0; 0];
covar_x = [1 0; 0 1];
rng(1);
x = randn(2,100000); %cada coluna é um dos 100000 pontos 2D
%Desnormalizar a VAR x que segue uma distribuição gaussiana 2D, com esperança 0 e matriz de
%covariancia identidade, por forma a obtermos a 
y = sqrtm(covar_y) * x + means_y;
%doc mean dá-me isto:
%M = mean(A,dim) returns the mean along dimension dim. For example, if A is a matrix, then mean(A,2) is a column vector containing the mean of each row.
means_y_sample = mean(y, 2);        
covar_y_sample = cov(y');	%cov(y) da-me erro e diz que usa 75GB de memoria (claramente errado), mas porque cov(y')?
disp('Estimativa da esperança de y:')
disp(means_y_sample)
disp('Estimativa da matriz de covariância y:')
disp(covar_y_sample)
%Vemos que as estimativas das médias e da matriz de covariância de y são
%aproximadas dos valores teóricos esperados.
correlation_matrix_y = corrcoef(y');
disp('Matriz de correlação de y (estimada): ')
disp(correlation_matrix_y)
%y1 esta directamente relacionada com si propria, assim (1,1) = 1, idem
%aspas para y2 e (2,2). y1 e y2 estão fortemente relacionadas de forma
%positiva entre si, com um coeficiente estimado de aproximadamente 5/9
r = 4;
teta=(0:100)/100*2*pi; cx=r*cos(teta);cy=r*sin(teta);
fig4 = figure(4);
%y = sqrtm(covar_y) * x + means_y;
%Aplicar esta transformação que aplicamos a y a cx e a cy
c = [cx; cy];
c = sqrtm(covar_y) * c + means_y;
%cy_transformado = sqrtm(covar_y) * cy + means_y;
plot(cx,cy, 'y', y(1,:), y(2,:), 'g.', x(1,:), x(2,:), 'r.', c(1,:), c(2,:), 'k'), axis equal, grid on, title('Scatter plot da amostra y com elipse de concentração de nivel r = 4'), legend('c','x', 'y', 'c transformado')
z = inv(sqrtm(covar_y))*(y - mean(y,2));
mean(z, 2)
cov(z')
%A média deverá ser aproximadamente 0 e a matriz de covariância é
%aproximadamente a matriz identidade











