function y = delayCh(x, d)
% Fun��o que dado um sinal vector coluna, x, devolve um sinal vector coluna, y, que � o sinal
% x com um atraso, d, de amostras, ie: d amostras iniciais nulas e sem d amostras finais

[n, m] = size(x);
if m ~= 1
    error('x deve ser um vector coluna.')
end

y = zeros(n, 1);        %Pr� alocar mem�ria e meter d amostras iniciais nulas
y(d+1:1:end) = x(1:1:end-d);      %copiar todos os valores de x para y, excepto os ultimos d valores