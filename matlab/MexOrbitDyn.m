% �������ѧ����mex����,�������ǵĹ������ѧ����,������ͬʱ���ƶ�����ǵĹ��
% out = orbitdyn(satllite_name,operator,parameter,...)
% satllite_name: ��������
% operator: ����,����Ϊ(�����ִ�СС):
%     'init':��ʼ��һ������
%     'step':����һ��(֧����ǰ��)
%     'Propagate':����һ��ʱ��
%     'PropagateBackward':��ǰ����һ��ʱ��
%     'Propagate2Perigee':���Ƶ����ص�
%     'Propagate2Apogee':���Ƶ�Զ�ص�
%     'Propagate2AscendingNode':���Ƶ�������
%     'Propagate2DescendingNode':���Ƶ�������
%     'Propagate2Equator':���Ƶ�������������
%     'Impulse': ���ϵ����������
%     'remove':ɾ��һ������
%     'SetForce':�����㶯��
%     'GetMean':���ع��ƽ����
%     'GetECF':���صع��µ�λ���ٶ�
%     'SetMass':������������
%     'SetSRP':���ù�ѹ���
%     'SetAD': ���ô����������
% parameter: ��Ӧoperator�Ĳ���
% ���:outΪһ������,����J2000�µĹ��������λ���ٶ�,�����е���������Ϊ:
%      ����ʱt(MJD), �볤��a(km), ƫ����e, ���i(rad),
%      ������ྭOmega(rad),���ص����w(rad),ƽ�����M(rad),
%      λ��x(km),λ��y(km),λ��z(km),�ٶ�x(km/s),�ٶ�y(km/s),�ٶ�z(km/s)
%
% ���ֲ�����Ӧ�Ĳ���Ϊ:
%     'init':����parameterΪһ������,����Ϊ:[��,��,��,ʱ,��,��,�볤��a(km),ƫ����e,���i(deg)
%                                  ,������ྭOmega(deg),���ص����w(deg),ƽ�����M(deg),��������(kg)]
%     'step':����һ��,֧����ǰ��
%            �����б��ʱ,����parameterΪ���������������ɸ���
%            ���й������ʱ����Ҫ�ṩ�Ĳ�����Ҫ������̬�������������仯��,��ʽ���£�
%            [����  qx  qy  qz  qs  Fbx  Fby  Fbz  dot_m]
%            qx  qy  qz  qs Ϊ���Ǳ���ϵ��Թ���ϵ����Ԫ��
%            һ̨����������ʱ�����仯��Ϊ dot_m = - F/(Isp*9.8)
%            ��̨������ͬʱ����ʱ������Fbx  Fby  FbzΪ���Ǳ���ϵ�ĺϳ������������仯��Ϊ���������ĺ�
%     'Propagate':����һ��ʱ��,����parameterΪʱ��(ʱ��Ϊ��)
%     'PropagateBackward':��ǰ����һ��ʱ��,����parameterΪʱ��(ʱ��Ϊ��)
%     'Propagate2Perigee':�޲���
%     'Propagate2Apogee':�޲���
%     'Propagate2AscendingNode':�޲���
%     'Propagate2DescendingNode':�޲���
%     'Propagate2Equator':�޲���
%     'Impulse': ���ϵ����������������Ϊ�����ٶ���������λkm/s
%     'remove':�޲���
%     'SetForce':�����㶯�����ͬC++�����е�SetForce()������Ϊ[p1,p2],p1Ϊ������������
%                     p2Ϊ�����㶯��㶯��ͬC++�еĺ궨�壬��Ӧ���������±�
%                     ��Ҫ���Ƕ����㶯ʱ���������Ӧ������͵õ�p2
%                                 1:��г��
%                                 2:��г��
%                                 4:��������
%                                 8:��������
%                                 16:̫������
%                                 32:̫����ѹ
%                    �㶯���õ�Ĭ��ֵΪ6,ODP_EARTH_ALL
%     'SetSRPCODE':����CODE��ѹģ�Ͳ���������Ϊһ��3*3�Ĺ�ѹϵ������
%     'GetMean':�޲��������ع��ƽ����
% 
% һ����ʼ��һ������,����һֱ���������ڴ���,ֱ��ִ����remove��������ִ��������clear all������clear orbitdyn
%
% �����ʼ��һ���Ѿ����ڵ�����,�����ظ�ʹ��,��������������ʾ
%
% Example:
%   ��ʼ��һ����Ϊsat������:
%   initparam = [2010,1,1,0,0,0,7044,0.001,98.5,270,90,30];
%   out = orbitdyn('sat','init',initparam);
%   ���ù���㶯ģ��Ϊ������J2��
%   orbitdyn('sat','SetForce',[2 1]);
%   ��'sat'���ǵĹ������60��:
%   out = orbitdyn('sat','step',60);
%   ִ����������������X����1m/s
%   out = orbitdyn('sat','impulse',[1,0,0]/1000)

% ʾ����
% epoch = [2015,1,1,4,0,0];
% kp = [24371.137,0.73,28,0,0,0];
% F = 0.2;
% Isp = 2400;
% dm = -F/Isp/9.8;
% Mass = 2000;
% s1 = mexOrbitDyn('sat','init',[epoch,kp,Mass]);
% mexOrbitDyn('sat','SetForce',[8,1+2+4+8+16+32]);
% mexOrbitDyn('sat','SetAD',20); % �������
% mexOrbitDyn('sat','SetSRP',20); % ��ѹ���
% step = 60;
% for i = 1:1000
%     qbi = c2q(getcoi(s1(2:7)));
%     s1 = mexOrbitDyn('sat','step',[step, qbi', 0, 0, F, dm]);
% end