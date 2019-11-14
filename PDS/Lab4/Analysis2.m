function [v0,v1] = Analysis2(h0,h1,x)
%[v0,v1] = Analysis2(h0,h1,x)
%Aplica x[n] ao banco de filtros de 2 canais com respostas a impulso
%h0 e h1 e decima as saídas por 2. Retorna estas saídas, v0 e v1, com metade
%do comprimento de x. A entrada x deve ser um vetor coluna com comprimento par.

[n, m] = size(x);
if m ~= 1
    error('x deve ser um vector coluna.')
end
if mod(n,2) ~= 0
    error('x deve ter um comprimento par')
end

if any(size(h0) ~= size(h1))
    error('h0 e h1 tem comprimentos diferentes')
end

%O numerador é 1 pois o filtro é FIR, e o denominador sao os coeficientes do filtro
x0 = filter(h0, 1, x);      
x1 = filter(h1, 1, x);
%Decimar por 2
v0 = downsample(x0, 2);         
v1 = downsample(x1, 2);



