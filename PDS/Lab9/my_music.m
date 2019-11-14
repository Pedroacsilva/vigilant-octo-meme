function Px = my_music(R,n_exp)
%Algorimo MUSIC, [Hayes, 1996, pp 465] (adaptado)
%R: matriz de autocorrela��o do processo.
%nexp: n�mero de exponenciais harm�nicas do processo.
%Retorna P, o pseudoespetro, em 1025 pontos, wn=(0:1024)/1024
%Faz plot do psedoespetro com 1025 pontos.
[M,MM]=size(R);
if (MM~=M),error('R n�o � matriz quadrada'); end
if n_exp>M-1, error('pelo menos uma exponencial harm�nica'); end
[V,D]=eig(R);
%lambda=diag(D); % valores pr�prios, lambda: diagonal da matriz D.
Px=0;
for j=1:M-n_exp
Px=Px+abs(fft(V(:,j),2048)).^2; %V � a matriz dos vetores pr�prios.
end
Px=1./Px;
wn=(0:1024)/1024; %frequ�ncia normalizada.
plot(wn, Px(1:1025)); grid on; xlabel('frequencia'); ylabel('P'); title('Pseudoespectro usando algoritmo MUSIC');%Faz plot