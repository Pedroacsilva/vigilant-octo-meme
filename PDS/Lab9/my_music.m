function Px = my_music(R,n_exp)
%Algorimo MUSIC, [Hayes, 1996, pp 465] (adaptado)
%R: matriz de autocorrelação do processo.
%nexp: número de exponenciais harmónicas do processo.
%Retorna P, o pseudoespetro, em 1025 pontos, wn=(0:1024)/1024
%Faz plot do psedoespetro com 1025 pontos.
[M,MM]=size(R);
if (MM~=M),error('R não é matriz quadrada'); end
if n_exp>M-1, error('pelo menos uma exponencial harmónica'); end
[V,D]=eig(R);
%lambda=diag(D); % valores próprios, lambda: diagonal da matriz D.
Px=0;
for j=1:M-n_exp
Px=Px+abs(fft(V(:,j),2048)).^2; %V é a matriz dos vetores próprios.
end
Px=1./Px;
wn=(0:1024)/1024; %frequência normalizada.
plot(wn, Px(1:1025)); grid on; xlabel('frequencia'); ylabel('P'); title('Pseudoespectro usando algoritmo MUSIC');%Faz plot