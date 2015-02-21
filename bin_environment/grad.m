% I = imread('disp_data/wall002_hmap2_512x512.jpg');
% I = imread('hi_noise.jpg');
% I = imread('preview_gravel-base-04_smoothed4_2m_p.jpg');
I = imread('wgnoise.jpg');
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

imshow(Ig);

[dx,dy] = imgradientxy(Ig);

dxx = dx .* dx;
dyy = dy .* dy;
dxy = dx .* dy;

% v = zeros(size(dx,1),size(dx,2));
% 
% dx = (dx)';
% dy = (dy)';
% dxy = (dxy)';
% dxx = (dxx)';
% 
% dyy = (dyy)';
% Ig = (Ig)';
% v = v';
% 
% dx = dx(:);
% dy = dy(:);
% dxy = dxy(:);
% dxx = dxx(:);
% 
% dyy = dyy(:);
% Ig = Ig(:);
% v = v(:);
% 
% 
% t1 = reshape([dx dy dxy dxx]',4*size(dx,1), []);
% t2 = reshape([dyy Ig v v]',4*size(dx,1), []);
% 
% tex1 = fopen('wall002_hmap2_512x512.leadr1','w');
% fwrite(tex1,size(I,1),'int32');
% fwrite(tex1,size(I,2),'int32');
% fwrite(tex1, t1, 'float32');
% 
% tex2 = fopen('wall002_hmap2_512x512.leadr2','w');
% fwrite(tex2,size(I,1),'int32');
% fwrite(tex2,size(I,2),'int32');
% fwrite(tex2,t2,'float32');
% 
% fclose(tex1);
% fclose(tex2);


