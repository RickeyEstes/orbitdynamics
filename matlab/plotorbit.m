%  �Թ������elemΪ��ֵ�����ն����˶�����һ��������ڵ���ά���
function rv = plotorbit(elem)
Color=rand(1,3);
while all(Color>0.4)
    Color=rand(1,3);
end    
rv = zeros(361,6);
for M = 0:1:360
    elem(6) = M*rad;
    rv(M+1,1:6) = kepler2cart(elem);
end
plot3(rv(:,1),rv(:,2),rv(:,3),'color','k'),grid on,hold on;