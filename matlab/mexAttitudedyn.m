% ��̬����ѧ����,�������ǵ���̬����ѧ����,������ͬʱ���ƶ�����ǵĹ��
% out = mexAttitudedyn(satllite_name,operator,parameter,...)
% satllite_name: ��������
% operator: ����,����Ϊ(�����ִ�СС):
%     'init':��ʼ��һ������
%     'step':����һ��(֧����ǰ��)
%     'Propagate':����һ��ʱ��
% parameter: ��Ӧoperator�Ĳ���
% ���:outΪһ������,����J2000�µĹ��������λ���ٶ�,�����е���������Ϊ:
%      ������Ԫ��qbi,���Խ��ٶ�wbi
%
% ���ֲ�����Ӧ�Ĳ���Ϊ:
%     'init':����parameterΪһ������,����Ϊ:[������Ԫ��qbi,���Խ��ٶ�wbi]
%     'step':����һ��
%            [����  Hwb  Tw  Tex]
%            HwbΪ������ϵ�ĺϳɽǶ���
%            TwΪ��ϵ�ĺϳ�����
%            TexΪ�������ϳ���������
%     'remove':�޲���
%     'SetIs':���ù���,����Ϊ3*3��������
% 
% һ����ʼ��һ������,����һֱ���������ڴ���,ֱ��ִ����remove��������ִ��������clear all������clear orbitdyn
%
% �����ʼ��һ���Ѿ����ڵ�����,�����ظ�ʹ��,��������������ʾ
%
% Example:
%   ��ʼ��һ����Ϊsat������:
%   att = [0 0 0 1 0.0 0.0 0.0];
%   mexAttitudedyn('a','init',att)
%   ��'sat'���ǵĹ������1��:
%   Hwb = [0 10 0];
%   Tw = [0 0.1 0];
%   Tew = [2 0.0 2];
%   mexAttitudedyn('a','step',[1,Hwb,Tw,Tew])

% ʾ����
% att = [0 0 0 1 0.0 0.0 0.0];
% mexAttitudedyn('a','init',att);
% clear all
% att = [0 0 0 1 0.0 0.0 0.0];
% mexAttitudedyn('a','init',att);
% mexAttitudedyn('a','getis')
% Is = magic(3)
% mexAttitudedyn('a','setis',Is)
% mexAttitudedyn('a','getis')
% Hwb = [0 10 0];
% Tw = [0 0.1 0];
% Tew = [2 0.0 2];
% mexAttitudedyn('a','step',[1,Hwb,Tw,Tew])
% Hwb = [0 10 0];
% Tw = [0 0.1 0];
% Tew = [2 0.0 2];
% mexAttitudedyn('a','step',[1,Hwb,Tw,Tew])