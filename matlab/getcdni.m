% ��������ϵ��Թ���ϵ����ת����
% cdni = getcdni(mjd,rv)
% ���룺 mjd: MJDʱ��,UTCʱ
%        rv:  J2000.0����ϵλ��
% ���: cdni������ϵ��Թ���ϵ��ת������
function cdni = getcdni(mjd,rv)
% ����J2000.0����������㷨
cfi = eci2ecf(mjd);
rv = rv(:);
rf = cfi*rv(1:3);
xfyf2 = sqrt(rf(1)^2+rf(2)^2);
rr = norm(rf);
cdni = [-rf(2)/xfyf2,rf(1)/xfyf2,0;...
    rf(1)*rf(3)/rr/xfyf2,rf(2)*rf(3)/rr/xfyf2,-xfyf2/rr;...
    -rf(1)/rr,-rf(2)/rr,-rf(3)/rr]*cfi;