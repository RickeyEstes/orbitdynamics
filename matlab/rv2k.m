% �ع�ϵλ���ٶ�ת������ϵ
function [ri,vi] = rv2k(c,rf,vf)
we = [0;0;We];
ri = c'*rf;
vi = c'*(vf + cross(we,rf));