alpha_b = 0.2;
theta_m = linspace(-pi/2.0, pi/2.0, 180);
theta_v = theta_m;

theta_m_deg = linspace(-90, 90, 180);
theta_v_deg = theta_m_deg;


D_Beckmann_ = D_Beckmann_1D(alpha_b, theta_m);
G_Beckmann_ = G1_Beckmann_1D(alpha_b, theta_v);

figure, plot(theta_m_deg, D_Beckmann_);

figure, plot(theta_v_deg, G_Beckmann_);