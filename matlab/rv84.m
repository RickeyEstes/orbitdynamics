% λ���ٶ�ת���ع�ϵ
function [rf,vf] = rv84(c,ri,vi)
we = [0;0;We];
rf = c*ri;
vf = c*vi - cross(we,rf);
