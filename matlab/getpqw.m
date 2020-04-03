% �ӹ���ϵ��PQW����ϵ��ת������
% c = getpqw(kepler)
% use:
%     kepler2cart(kepler);
%     keplerΪ[a e i Omega w M],��λΪkm,rad
% result:
%     c = [P';Q';W']
% See also: getcoi getcdni getunw
function c = getpqw(kepler)
if nargin == 0 %self test
    kepler = [7044,0.02,1.7,1,-1,2];
%     kepler = kepler';
end
sini = sin(kepler(3));
cosi = cos(kepler(3));
sino = sin(kepler(4));
coso = cos(kepler(4));
sinw = sin(kepler(5));
cosw = cos(kepler(5));
P = [coso.*cosw-sino.*sinw.*cosi,    sino.*cosw+coso.*sinw.*cosi,    sinw.*sini];
Q = [-coso.*sinw-sino.*cosw.*cosi,    -sino.*sinw+coso.*cosw.*cosi,    cosw.*sini];
W = cross(P,Q);
c = [P;Q;W];