function [G_] = G1_Beckmann_1D(alpha_b, theta_v) % Assuming theta_n == 0 (macronormal)

cos_theta_n = ones(1, length(theta_v)); % See assumption above

a = abs((alpha_b .* tan(theta_v)).^(-1));

Heavyside_part = Heavyside(cos(theta_v) ./ cos_theta_n);

%%% WALTER07 SIMPLIFIED VERSION %%%
filter = a < 1.6;
a = a(filter);
simp_part = ones(1, length(filter));
simp_part(filter) = (3.535 .* a + 2.181 .* (a.^2)) ./ (1 + 2.276 .* a + 2.577 .* (a.^2));

G_ = Heavyside_part .* simp_part;
%%% END WALTER07 %%%


%%% CLASSIC VERSION %%%
% div_part = (1 + erf(a) + (exp(-(a.^2)) ./ (a .* sqrt(pi))));
% 
% G_ = (2 .* Heavyside_part) ./ div_part;
%%% END CLASSIC %%%