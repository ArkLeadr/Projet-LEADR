function [] = computeStatistics(dx, dy, dxx, dyy, dxy)

global E_xn
global E_yn
global E_xn2
global E_yn2
global E_xnyn

global var_x
global var_y
global c_xy

E_xn    = mean(mean(dx))
E_yn    = mean(mean(dy))
E_xnyn  = mean(mean(dxy))

E_xn2   = mean(mean(dxx))
E_yn2   = mean(mean(dyy))


var_x = E_xn2 - E_xn*E_xn
var_y = E_yn2 - E_yn*E_yn

c_xy = E_xnyn - E_xn*E_yn

E_xn = 0;
E_yn = 0;
E_xn2 = 0;
E_yn2 = 0;
E_xnyn = 0;

baseRoughnessOffset = 0.1;

var_x = E_xn2 - E_xn*E_xn + baseRoughnessOffset;
var_y = E_yn2 - E_yn*E_yn + baseRoughnessOffset;

c_xy = E_xnyn - E_xn*E_yn;

end
