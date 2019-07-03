function [ cla ] = eqt2cla( eqt )
%EQT2CLA ��תСƫ���ʸ�����Ϊ����������
%   ����������Ϊ[a;e;i;Omega;w;M]
%   Сƫ���ʸ���Ϊ[a;ex;ey;i;Omega;lamda]
cla(1) = eqt(1);
cla(2) = norm(eqt(2:3));
cla(3) = eqt(4);
cla(4) = eqt(5);
cla(5) = atan2(eqt(3),eqt(2));
cla(6) = eqt(6) - cla(5);

end

