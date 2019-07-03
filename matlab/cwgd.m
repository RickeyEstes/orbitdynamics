% CW�Ƶ�
% [dv0,dvf,dvall] = cwguidance(r0,rf,T,w)
% �����ʼλ���ٶ�r0��ĩ��λ���ٶ�rf��ת��ʱ��T��Ŀ�������ٶ�w
% �����ʼ����dv0,ĩ����dvf,���ٶ�����dvall
function [dv0,dvf,dvall] = cwgd(r0,rf,T,w)
if nargin == 0
    warning('self test');
    r0 = [-3;0;0;0;0;0];
    rr = [rand1(3,100)*0.5;rand1(3,100)*0.0002];
    rf = [-1;0;0;0;0;0];
    w = sqrt(GEarth/7066^3);
    T = 4000;
    figure,cwgd(r0,rf,T,w);
    figure,cwgd(repmat(r0,1,100)+rr,rf,T,w);
    figure,cwgd(r0,rf,T+[-1000:200:1000],w);
    return
end
rf = rf(:);
h = 60;
nr = size(r0,2);
nT = length(T);
if nr > 1
    % ���㲻ͬ��㡢��ͬת��ʱ��Ĺ켣
    dv0 = nan(3,nr);
    dvf = nan(3,nr);
    dvall = nan(1,nr);
    for k=1:nr
        [dv0(:,k),dvf(:,k),dvall(k)] = cw2p(r0(:,k),rf,T,w);
        [traj,t] = cwtraj(r0(:,k)+[0;0;0;dv0(:,k)],w,T,h);
        plottraj(2);
    end
    subplot(2,2,[3,4]),hist(dvall*1000);
elseif nT > 1
    dv0 = nan(3,nT);
    dvf = nan(3,nT);
    dvall = nan(1,nT);
    % ���㲻ͬת��ʱ��Ĺ켣
    for k=1:nT
        [dv0(:,k),dvf(:,k),dvall(k)] = cw2p(r0,rf,T(k),w);
        [traj,t] = cwtraj(r0+[0;0;0;dv0(:,k)],w,T(k),h);
        plottraj(2);
    end
    subplot(2,2,[3,4]),hist(dvall*1000);
else
    % ֻ����һ���켣
    [dv0,dvf,dvall] = cw2p(r0,rf,T,w);
    [traj,t] = cwtraj(r0+[0;0;0;dv0],w,T,h);
    plottraj(1);
end

    function plottraj(n)
        subplot(2,n,1),plotreltraj(traj,t,'xz'),hold on;
        % plot(0,0,'r*');
        % plot(0.1*cosd(0:360),0.1*sind(0:360),'r');
        subplot(2,n,2),plotreltraj(traj,t,'xy'),hold on;
        % plot(0,0,'r*');
        % plot(0.1*cosd(0:360),0.1*sind(0:360),'r');
    end
end

