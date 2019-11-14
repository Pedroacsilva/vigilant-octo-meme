function y = Synthesis2(f0,f1,v0,v1)
%y = Synthesis2(f0,f1,v0,v1)
%Expande v0[n] e v1[n] por 2 e aplica-os ao banco de filtros de síntese de 2
%canais com respostas a impulso f0 e f1.
%As entradas v0 e v1 têm de ter o mesmo comprimento.
%Sintetiza a saída, y, num vetor coluna de comprimento duplo de v0 e v1.

if any(size(v0) ~= size(v1))
    error('v0 e v1 tem comprimentos diferentes')
end

%expandir sinais v0 e v1 por 2
y0 = upsample(v0, 2);
y1 = upsample(v1, 2);

%O numerador é 1 pois o filtro é FIR, e o denominador sao os coeficientes do filtro
y = filter(f0, 1, y0) + filter(f1, 1, y1);