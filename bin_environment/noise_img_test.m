I = wgn(2048,2048,-21) + 0.5;

min(min(I))
max(max(I))

imshow(I)

imwrite(I, 'wgnoise.jpg')