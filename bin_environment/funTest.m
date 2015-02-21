perfect_middle_index = 41;
lin_size = 2*perfect_middle_index - 1;


x = linspace(-1, 1, lin_size);
y = x';

[X,Y] = meshgrid(x, y);

computeStatistics(dx, dy, dxx, dyy, dxy);

% z = exp(-(X.^2+Y.^2)/2);

z = P22(X, Y);

% figure, surf(x, y, double(z));
% 
% figure, plot(x, z(perfect_middle_index, :));

figure, plot(y, z(:, perfect_middle_index));
