alpha_b = 0.2;
% theta_m = linspace(0, pi/2, 1800);
% theta_v = theta_m;

% theta_m_deg = linspace(-90, 90, 180);
% theta_v_deg = theta_m_deg;


% D_Beckmann_ = D_Beckmann_1D(alpha_b, theta_m);
 
% mean(D_Beckmann_)
% G_Beckmann_ = G1_Beckmann_1D(alpha_b, theta_v);
% 
% figure, plot(theta_m, D_Beckmann_);
% 
% figure, plot(theta_v_deg, G_Beckmann_);
% 
% integral(@(x)(sin(x).*x), 0, pi/2)
% 
% integral2(@(dtheta, dphi)(cos(dtheta).*sin(dtheta)), 0, pi/2, 0, 2*pi)

% mean(sin(theta_m))

% integral2(@(x, y)(x.*y.*0 + 1), 0, pi/2, 0, pi)

% integral(@(x)( D_Beckmann_1D(alpha_b, x).*2.*cos(x).*sin(x)), 0, pi/2)

% computeStatistics(dx, dy, dxx, dyy, dxy, alpha_b);

% integral(@(x)( D(0, x).*2.*cos(x).*sin(x)), 0, pi/2)

% m_giving_one = 0.193418;

integral(@(t)( 2 .* t .* Dt(t, 0.2) ), 0, 1) % == 1

integral(@(alpha)( 2 .* cos(alpha) .* Dt(cos(alpha), 0.2) ) .* sin(alpha), 0, 1) % == 1

% integral2(@(theta, phi)(D(theta, phi) .* cos(theta) .* sin(theta)), 0, pi/2, 0, 2*pi) % == pi