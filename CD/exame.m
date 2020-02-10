%Pergunta 1
beta = 0; alfa = 0.8; h = 0.5;
A=[-3 0; 1 0];
B =[1; 0];
C = [ beta, alfa];
D = 0;
[phi, gamma] = c2d(A, B, h)
Wo = [C; C * phi]
%Observavel?
rank(Wo)
%Sim, rank = 2
%b)
beta = 1; alfa = 0;
C = [ beta, alfa];
[phi, gamma] = c2d(A, B, h)     %Mesmo resultado?...
Wo = [C; C * phi]
rank(Wo)
%c)
beta = 0; alfa = 0.8; h = 0.5;
C=[beta alfa];
wn = 1; wnh = 0.5;
ksi = 1;
p1 = -2 * exp(-ksi * wnh) * cos(wnh * sqrt(1 - ksi^2));      %polos caracteristicos
p2 = exp(-2 * ksi *wnh);
Wc = [gamma, phi * gamma];
L = [0  1] * inv(Wc) * (phi^2 + p1 * phi + p2 * eye(2));
phi_clreg = phi - gamma * L;

%Lol simulink
%d1)
Da = [0; 0]; Ca = eye(2);
Gdc = 1;
phi_cl = phi - gamma * L;
Lc = Gdc / (C * inv((eye(2) - phi_cl)) * gamma);
%2a)
A = [-3 0; 1 0];
B = [1; 0];
C = [0 0.8];
z1 = 0.12;
z2 = 0.12;
ksi = 1;
wn = 1;
P = phi^2 + p1 * phi + p2 * eye(2);
Wo =[ C ; C * phi];
Wo_phi = Wo * phi;
K = P * inv(Wo_phi) * [0; 1];
%2b)
%Pagina 197
phi_cl_ctrobs = [0.3051, -0.3069, -0.0860, 0.3096;
                0.2856, 0.9036, -0.0267, 0.0961;
                0, 0, -0.4357, -2.5440;
                0, 0, 0.4269, 1.6487];





