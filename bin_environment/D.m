function [D_] = D(x, y)

P22_ = P22(x, y);

D_ = P22_;

% z = 1 - sqrt(1 - x.*x - y.*y);
% 
% D_ = z;