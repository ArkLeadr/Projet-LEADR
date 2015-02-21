function [P22_] = P22(x, y)

global E_xn
global E_yn
global E_xnxn
global E_ynyn
global E_xnyn

global var_x
global var_y
global c_xy


x = x - E_xn;
y = y - E_yn;

det = var_x .* var_y - c_xy .* c_xy ;

% det = max(det, 0.00001);

% det(det == 0) = 1;

% if det == 0
%     det = 1;
% end

arg_exp = -0.5 .* ( x .* x .* var_y + y .* y * var_x - 2.0 .* x .* y .* c_xy ) ./ det;

P22_ = 0.15915 ./ sqrt( det ) .* exp ( arg_exp );
