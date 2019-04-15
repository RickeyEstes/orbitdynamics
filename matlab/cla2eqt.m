function [ eqt ] = cla2eqt( cla )
%CLA2EQT ������������ת��ΪСƫ���ʸ���
%   ����������Ϊ[a;e;i;Omega;w;M]
%   Сƫ���ʸ���Ϊ[a;ex;ey;i;Omega;lamda]
eqt(1,1) = cla(1);
eqt(2,1) = cla(2)*cos(cla(5));
eqt(3,1) = cla(2)*sin(cla(5));
eqt(4,1) = cla(3);
eqt(5,1) = cla(4);
eqt(6,1) = mod(cla(5) + cla(6),2*pi);
end

