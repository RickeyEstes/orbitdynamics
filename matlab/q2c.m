% ��Ԫ�������������
function C = q2c(q)
narginchk(1,1);
C = [1.0 - 2.0*(q(2)^2+q(3)^2),    2.0*(q(1)*q(2)+q(3)*q(4)),    2.0*(q(1)*q(3)-q(2)*q(4));
     2.0*(q(1)*q(2)-q(3)*q(4)),    1.0 - 2.0*(q(1)^2+q(3)^2),    2.0*(q(2)*q(3)+q(1)*q(4));
     2.0*(q(1)*q(3)+q(2)*q(4)),    2.0*(q(2)*q(3)-q(1)*q(4)),    1.0 - 2.0*(q(1)^2+q(2)^2)];