%Lab 7 PDS: Vari�veis aleat�rias, histogramas e normaliza��o de amostras.
%PL2
%Grupo 8
%Autor:
%Pedro Silva
% Neste trabalho pretende-se elaborar os conceitos de vari�vel aleat�ria (VA), amostra, fun��o
% densidade de probabilidade e normaliza��o de vari�veis aleat�rias. Para isso, iremos usar os dois
% geradores de n�meros pseudoaleat�rios do Matlab: rand (distribui��o uniforme entre 0 e 1) e randn
% (distribui��o normal ou Gaussiana, de m�dia zero e vari�ncia 1). Os n�meros gerados s�o
% pseudoaleat�rios pois podemos repetir a amostra inicializando o gerador no mesmo estado
% (rng(seed)).
close all; clc; clear; mkdir('Imagens '); delete Imagens/*.*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 1. Amostra 1D com distribui��o triangular.
rng(0) %para o caso de querermos repetir a experi�ncia.
x1 = rand(1,1e5); %uma amostra com 10^5 realiza��es
x2 = rand(1,1e5); %outra: independente e identicamente distribu�da.
x_tri = x1+x2;
fig1 = figure(1);
histogram(x_tri, 100, 'Normalization', 'pdf'); title('Histograma de X (distribui��o triangular)');
print(fig1, '-dpng', 'Imagens/histograma_X')
MSG = ['Estimativa da m�dia de x: ', num2str(mean(x_tri)),'. Estimativa da vari�ncia de x: ', num2str(var(x_tri)),'.'];
disp(MSG)
%Matematicamente, sabemos que a m�dia de X_tri � 1 e a vari�ncia � 1/6
fig2 = figure(2);
subplot(2,1,1)
histogram(x1, 100, 'Normalization', 'pdf'); title('Histograma da VA x1');
subplot(2,1,2)
histogram(x2, 100, 'Normalization', 'pdf'); title('Histograma da VA x2');
print(fig2, '-dpng', 'Imagens/histogramas_x1_x2')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 2. Amostra 1D com distribui��o Gaussiana.
rng(0) %para o caso de querermos repetir a experi�ncia.
x_gauss = 7*randn(1,100000)+3; %amostra com 100k pontos
%z segue uma distribui��o gaussiana com esperan�a 0 e variancia 1.
%Assim, x = z + 3 tem esperan�a 3 e variancia=
%E{(x-3)^2}=E{(7z)^2}=7^2*E{z}=49. 
%A nossa distribui��o tem esperan�a 3 e variancia 49.
fig3 = figure(3);
h = histogram(x_gauss, 100, 'Normalization', 'pdf');
mean_x_gauss = mean(x_gauss);
var_x_gauss = var(x_gauss);
MSG = ['Estimativa da m�dia de x gauss: ', num2str(mean_x_gauss),'. Estimativa da vari�ncia de x gauss: ', num2str(var_x_gauss),'.'];
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
% 3. Vetor de duas vari�veis aleat�rias
means_y  = [27; 7];
covar_y = [4 5; 5 9];
means_x = [0; 0];
covar_x = [1 0; 0 1];
rng(1);
x = randn(2,100000); %cada coluna � um dos 100000 pontos 2D
%Desnormalizar a VAR x que segue uma distribui��o gaussiana 2D, com esperan�a 0 e matriz de
%covariancia identidade, por forma a obtermos a 
y = sqrtm(covar_y) * x + means_y;
%doc mean d�-me isto:
%M = mean(A,dim) returns the mean along dimension dim. For example, if A is a matrix, then mean(A,2) is a column vector containing the mean of each row.
means_y_sample = mean(y, 2);        
covar_y_sample = cov(y');	%cov(y) da-me erro e diz que usa 75GB de memoria (claramente errado), mas porque cov(y')?
disp('Estimativa da esperan�a de y:')
disp(means_y_sample)
disp('Estimativa da matriz de covari�ncia y:')
disp(covar_y_sample)
%Vemos que as estimativas das m�dias e da matriz de covari�ncia de y s�o
%aproximadas dos valores te�ricos esperados.
correlation_matrix_y = corrcoef(y');
disp('Matriz de correla��o de y (estimada): ')
disp(correlation_matrix_y)
%y1 esta directamente relacionada com si propria, assim (1,1) = 1, idem
%aspas para y2 e (2,2). y1 e y2 est�o fortemente relacionadas de forma
%positiva entre si, com um coeficiente estimado de aproximadamente 5/9
r = 4;
teta=(0:100)/100*2*pi; cx=r*cos(teta);cy=r*sin(teta);
fig4 = figure(4);
%y = sqrtm(covar_y) * x + means_y;
%Aplicar esta transforma��o que aplicamos a y a cx e a cy
c = [cx; cy];
c = sqrtm(covar_y) * c + means_y;
%cy_transformado = sqrtm(covar_y) * cy + means_y;
plot(cx,cy, 'y', y(1,:), y(2,:), 'g.', x(1,:), x(2,:), 'r.', c(1,:), c(2,:), 'k'), axis equal, grid on, title('Scatter plot da amostra y com elipse de concentra��o de nivel r = 4'), legend('c','x', 'y', 'c transformado')
z = inv(sqrtm(covar_y))*(y - mean(y,2));
mean(z, 2)
cov(z')
%A m�dia dever� ser aproximadamente 0 e a matriz de covari�ncia �
%aproximadamente a matriz identidade











