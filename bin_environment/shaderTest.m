I = imread('disp_data/wall002_hmap2_512x512.jpg');
% I = imread('hi_noise.jpg');
% I = imread('preview_gravel-base-04_smoothed4_2m_p.jpg');

% I = imread('9.jpg');

if size(I, 3) == 3
    Ig = single(rgb2gray(I));
else
    Ig = single(I);
end

% imshow(Ig / 255.0);
Ig = Ig / 255.0; % (Ig-min(min(Ig)))/((max(max(Ig))-min(min(Ig))));

% Ig = zeros(512);

% Ig(200:300, 200:300) = 0.5;



Ig = flip(Ig);

[dx,dy] = imgradientxy(Ig);

dxx = dx .* dx;
dyy = dy .* dy;
dxy = dx .* dy;


% global E_xn
% global E_yn
% global E_xn2
% global E_yn2
% global E_xnyn
% 
% global var_x
% global var_y
% global c_xy

% E_xn = mean(mean(dx));
% E_yn = mean(mean(dy));
% E_xn2 = mean(mean(dxx));
% E_yn2 = mean(mean(dyy));
% E_xnyn = mean(mean(dxy));

E_xn = dx;
E_yn = dy;
E_xn2 = dxx;
E_yn2 = dyy;
E_xnyn = dxy;

var_x = E_xn2 - E_xn.*E_xn;
var_y = E_yn2 - E_yn.*E_yn;

c_xy = E_xnyn - E_xn.*E_yn;

x = 0 - E_xn;
y = 0 - E_yn;

det = var_x .* var_y - c_xy .* c_xy;

det_zeros = det;

det(det == 0) = 1;
% if det == 0
%     det = 1;
% end

arg_exp = -0.5 .* ( x .* x .* var_y + y .* y * var_x - 2.0 .* x .* y .* c_xy ) ./ det;

P22_ = 0.15915 ./ sqrt( abs(det) ) .* exp ( arg_exp );

% P22_ = bsxfun(@(x, y) x == 1

P22_(det_zeros == 0) = (and(x == 0, y == 0));

imshow(P22_)

% figure('Name', 'E_xn'), imshow(flip(E_xn));
% figure('Name', 'E_yn'), imshow(flip(E_yn));
% figure('Name', 'E_xnyn'), imshow(flip(E_xnyn));
% figure('Name', 'E_xn2'), imshow(flip(E_xn2));
% figure('Name', 'E_yn2'), imshow(flip(E_xn2));
% figure('Name', 'disp'), imshow(flip(Ig));



% testM = ones(512);
% figure, imshow(testM);
% 
% testM(:,:) = 0;
% figure, imshow(testM);
% 
% testM(:,:) = -1;
% figure, imshow(testM);

% E_yn(1,1:10)

