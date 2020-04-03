% ���������վ�Ŀɼ���
% [v,az,el] = access(sr,flla)
% sr:�����ڵع�ϵ�е�λ��
% flla������վ���ȡ�γ�Ⱥ͸߶�
% v=0,1: �ɼ��ԣ�Ĭ����С����3��
% el: ����
function [v,az,el] = access(sr,flla)
if nargin == 0
    warning('self test');
    lon = 20;
    lat = 20;
    sr = (Re+500)*[cosd(lat)*cosd(lon);cosd(lat)*sind(lon);sind(lat)];
    flla = [20,20,0];
    [v,az,el] = access(sr,flla)
    return
end   
sr = sr(:);
c = NES(flla(1),flla(2));
% f = 1.0/298.2572236;
% ee = 1-(1-f)*(1-f);
N = 6378.137/sqrt(1-0.0066944*sind(flla(2))*sind(flla(2)));
ECFPosition = [(N+flla(3))*cosd(flla(2))*cosd(flla(1));...
               (N+flla(3))*cosd(flla(2))*sind(flla(1));...
               (N*(1-0.0066944)+flla(3))*sind(flla(2))];
% ����վ-����ʸ���ڵ���վ����������ϵ�ķ���
s = c*(sr - ECFPosition);
rou = norm(s);
az = atan2d(s(2),s(1));
el = asind(s(3)/rou);
v = 0;
if el > 3
    v = 1;
end
