%% �������ݣ����figure
function h = figurede(varargin)
h = figure;
set(h,'Position',[460   320   540*1.5   410*1.5]);
set(zoom,'Motion','vertical','Enable','on');
set(h,'Toolbar','figure');
for i=1:2:length(varargin)
    set(h,varargin{i},varargin{i+1});
end
% �����л��İ�ť
p = get(h,'Position');
x = (p(3)-300)/2;
y = 0;%(p(4)-30)/2;
uicontrol('Style','CheckBox','String','��ʵֵ����',...
    'pos',[x y 100 30],'parent',h,'HandleVisibility','off',...
    'FontSize',12,'FontName','����','Value',1,...
    'Callback',@checktrue,'Tag','CheckBoxTrueValue');
uicontrol('Style','CheckBox','String','����ֵ����',...
    'pos',[x+100 y 100 30],'parent',h,'HandleVisibility','off',...
    'FontSize',12,'FontName','����','Value',1,...
    'Callback',@checkest,'Tag','CheckBoxEstValue');
uicontrol('Style','CheckBox','String','�������',...
    'pos',[x+200 y 100 30],'parent',h,'HandleVisibility','off',...
    'FontSize',12,'FontName','����','Value',0,...
    'Callback',@checkerror,'Tag','CheckBoxErrorValue');

handles = guidata(gcf);
handles.figurede = 1;
guidata(gcf,handles);

    function checktrue(hObject,~)
        ud = guidata(hObject);
        if (get(hObject,'Value') == get(hObject,'Max'))
            for kk=1:ud.nlines
                set(ud.tru(kk),'Visible','On');
            end
        else
            for kk=1:ud.nlines
                set(ud.tru(kk),'Visible','Off');
            end
        end
    end
    function checkest(hObject,~)
        ud = guidata(hObject);
        if (get(hObject,'Value') == get(hObject,'Max'))
            for kk=1:ud.nlines
                set(ud.est(kk),'Visible','On');
            end
        else
            for kk=1:ud.nlines
                set(ud.est(kk),'Visible','Off');
            end
        end
    end
    function checkerror(hObject,~)
        ud = guidata(hObject);
        if (get(hObject,'Value') == get(hObject,'Max'))
            for kk=1:ud.nlines
                set(ud.err(kk),'Visible','On');
            end
        else
            for kk=1:ud.nlines
                set(ud.err(kk),'Visible','Off');
            end
        end
    end
end