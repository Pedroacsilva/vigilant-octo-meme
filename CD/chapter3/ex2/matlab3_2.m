%CD; pag 69, ex 3.2
close all; clc; clear; mkdir('Imagens '); delete Imagens/*.*
numG = [2.05]; denG = [1 3.74 0];
G = tf(numG, denG);
%queremos que tenha ksi = 0.5 e wn = 2 rad/s, para isso vamos usar o metodo
%analitico para sintonizar um PID

%Introduzir um polo nao dominante em -10 (p = 10)
ksi = 0.5;
wn = 2;
p = 10;
Kp = (2 * p * ksi * wn + wn^2 - denG(3))/numG(1);
Kd = (2 * ksi * wn + p - denG(2))/numG(1);
Ki = (p * wn^2)/numG(1);

simulacao = sim('sim3_2')
y = simulacao.y.signals.values;
deriv_comp = simulacao.deriv_comp.signals.values;
fig1 = figure(1)
subplot(2,1,1)
plot(y)
title('Saída (p = 10)'); xlabel('tempo (s)'); ylabel('y(t)');
subplot(2,1,2)
plot(deriv_comp)
title('Componente Derivativa (p=10)'); xlabel('tempo (s)'); ylabel('comp_deriv(t)');
print(fig1, '-dpng', 'Imagens/saida_p_10');

%Repetir para p = 100 e p = 5;
%p = 100;
p = 100;
Kp = (2 * p * ksi * wn + wn^2 - denG(3))/numG(1);
Kd = (2 * ksi * wn + p - denG(2))/numG(1);
Ki = (p * wn^2)/numG(1);
simulacao = sim('sim3_2')
y = simulacao.y.signals.values;
deriv_comp = simulacao.deriv_comp.signals.values;
fig2 = figure(2)
subplot(2,1,1)
plot(1:length(y), y)
title('Saída (p = 100)'); xlabel('tempo (s)'); ylabel('y(t)');
subplot(2,1,2)
plot(deriv_comp)
title('Componente Derivativa (p=10)'); xlabel('tempo (s)'); ylabel('comp_deriv(t)');
print(fig2, '-dpng', 'Imagens/saida_p_100');
%p = 5
p = 5;
Kp = (2 * p * ksi * wn + wn^2 - denG(3))/numG(1);
Kd = (2 * ksi * wn + p - denG(2))/numG(1);
Ki = (p * wn^2)/numG(1);
simulacao = sim('sim3_2')
y = simulacao.y.signals.values;
deriv_comp = simulacao.deriv_comp.signals.values;
fig3 = figure(3)
subplot(2,1,1)
plot(1:length(y), y)
title('Saída (p = 5)'); xlabel('tempo (s)'); ylabel('y(t)');
subplot(2,1,2)
plot(deriv_comp)
title('Componente Derivativa (p=5)'); xlabel('tempo (s)'); ylabel('comp_deriv(t)');
print(fig3, '-dpng', 'Imagens/saida_p_5');

%Repetir mas suavizar a entrada, meter um integrador + saturador
%p = 5; entrada suavizada
simulacao = sim('sim3_2_step_sat')
y = simulacao.y.signals.values;
deriv_comp = simulacao.deriv_comp.signals.values;
fig4 = figure(4)
subplot(2,1,1)
plot(1:length(y), y)
title('Saída (p = 5), entrada saturada'); xlabel('tempo (s)'); ylabel('y(t)');
subplot(2,1,2)
plot(deriv_comp)
title('Componente Derivativa (p=5), entrada saturada'); xlabel('tempo (s)'); ylabel('comp_deriv(t)');
print(fig4, '-dpng', 'Imagens/saida_p_5_step_sat');
%p = 100; entrada suavizada
p = 100;
Kp = (2 * p * ksi * wn + wn^2 - denG(3))/numG(1);
Kd = (2 * ksi * wn + p - denG(2))/numG(1);
Ki = (p * wn^2)/numG(1);
simulacao = sim('sim3_2_step_sat')
y = simulacao.y.signals.values;
deriv_comp = simulacao.deriv_comp.signals.values;
fig5 = figure(5)
subplot(2,1,1)
plot(y)
title('Saída (p = 100), entrada saturada'); xlabel('tempo (s)'); ylabel('y(t)');
subplot(2,1,2)
plot(deriv_comp)
title('Componente Derivativa (p=100), entrada saturada'); xlabel('tempo (s)'); ylabel('comp_deriv(t)');
print(fig5, '-dpng', 'Imagens/saida_p_100_step_sat');
%p = 10, entrada suavizada
p = 10;
Kp = (2 * p * ksi * wn + wn^2 - denG(3))/numG(1);
Kd = (2 * ksi * wn + p - denG(2))/numG(1);
Ki = (p * wn^2)/numG(1);
simulacao = sim('sim3_2_step_sat')
y = simulacao.y.signals.values;
deriv_comp = simulacao.deriv_comp.signals.values;
fig6 = figure(6)
subplot(2,1,1)
plot(y)
title('Saída (p = 10), entrada saturada'); xlabel('tempo (s)'); ylabel('y(t)');
subplot(2,1,2)
plot(deriv_comp)
title('Componente Derivativa (p=10), entrada saturada'); xlabel('tempo (s)'); ylabel('comp_deriv(t)');
print(fig6, '-dpng', 'Imagens/saida_p_10_step_sat');

%p = 10, entrada suavizada, derivada modificada
simulacao = sim('sim3_2_step_sat_d_mod')
y = simulacao.y.signals.values;
control_signal = simulacao.control_signal.signals.values;
fig7 = figure(7)
subplot(2,1,1)
plot(y)
title('Saída (p = 10), entrada saturada, derivada modificada'); xlabel('tempo (s)'); ylabel('y(t)');
subplot(2,1,2)
plot(control_signal)
title('Sinal Controlo, p = 10, entrada saturada, derivada modificada'); xlabel('tempo (s)'); ylabel('control signal(t)');
print(fig7, '-dpng', 'Imagens/saida_p_10_step_sat_d_mod');

%p = 5, entrada suavizada, derivada modificada
p = 5;
Kp = (2 * p * ksi * wn + wn^2 - denG(3))/numG(1);
Kd = (2 * ksi * wn + p - denG(2))/numG(1);
Ki = (p * wn^2)/numG(1);

simulacao = sim('sim3_2_step_sat_d_mod')
y = simulacao.y.signals.values;
control_signal = simulacao.control_signal.signals.values;
fig8 = figure(8)
subplot(2,1,1)
plot(y)
title('Saída (p = 5), entrada saturada, derivada modificada'); xlabel('tempo (s)'); ylabel('y(t)');
subplot(2,1,2)
plot(control_signal)
title('Sinal Controlo, p = 5, entrada saturada, derivada modificada'); xlabel('tempo (s)'); ylabel('control signal(t)');
print(fig8, '-dpng', 'Imagens/saida_p_5_step_sat_d_mod');

%p = 100, entrada suavizada, derivada modificada
p = 100;
Kp = (2 * p * ksi * wn + wn^2 - denG(3))/numG(1);
Kd = (2 * ksi * wn + p - denG(2))/numG(1);
Ki = (p * wn^2)/numG(1);

simulacao = sim('sim3_2_step_sat_d_mod')
y = simulacao.y.signals.values;
control_signal = simulacao.control_signal.signals.values;
fig9 = figure(9)
subplot(2,1,1)
plot(y)
title('Saída (p = 100), entrada saturada, derivada modificada'); xlabel('tempo (s)'); ylabel('y(t)');
subplot(2,1,2)
plot(control_signal)
title('Sinal C4ontrolo, p = 100, entrada saturada, derivada modificada'); xlabel('tempo (s)'); ylabel('control signal(t)');
print(fig8, '-dpng', 'Imagens/saida_p_100_step_sat_d_mod');



