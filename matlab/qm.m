% ��Ԫ���˷� q = q1*q2
% q = Qim(q1,q2)
function q = qm(q1,q2)
error(nargchk(2,2,nargin));
q = [q2(4),  q2(3), -q2(2), q2(1);
    -q2(3),  q2(4),  q2(1), q2(2);
     q2(2), -q2(1),  q2(4), q2(3);
    -q2(1), -q2(2), -q2(3), q2(4)]*q1;
%q = [q1(4), -q1(3),  q1(2), q1(1);
%     q1(3),  q1(4), -q1(1), q1(2);
%    -q1(2),  q1(1),  q1(4), q1(3);
%    -q1(1), -q1(2), -q1(3), q1(4)]*q2;
