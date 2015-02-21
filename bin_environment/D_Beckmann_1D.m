function [D_] = D_Beckmann_1D(alpha_b, theta_m)

Heavyside_part = Heavyside(cos(theta_m));
exp_part = exp((-tan(theta_m).^2) ./ alpha_b.^2);  
div_part = (pi .* alpha_b.^2 .* cos(theta_m).^4);

D_ = (Heavyside_part .* exp_part) ./ div_part;