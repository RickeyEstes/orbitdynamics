#include <stdio.h>
#include <math.h>
#include <conio.h>
#define PI 3.141592
  float A,B,C,a,b,c;

void getab() /* ��֪A,B,C,c�������ҹ�ʽ��a,b */
{ float tt,aa,bb;
	tt=sin(C)/sin(c);
	aa=tt*sin(A);bb=tt*sin(B);
	if(aa>=-1&&aa<0) a=asin(aa)+PI/2;
	else if(aa>=0&&aa<=1) a=asin(aa);
	if(bb>=-1&&bb<0) b=asin(bb)+PI/2;
	else if(bb>=0&&bb<=1) b=asin(bb);
}

void getAB()  /* ��֪a,b,c,C,�����ҹ�ʽ��A,B */
{ float tt,aa,bb;
	tt=sin(C)/sin(c);
	aa=tt*sin(a); bb=tt*sin(b);
	if(aa>=-1&&aa<0) A=asin(aa)+PI/2;
	else if(aa>=0&&aa<=1) A=asin(aa);
	if(bb>=-1&&bb<0) B=asin(bb)+PI/2;
	else if(bb>=0&&bb<=1) B=asin(bb);
}

void getCc()  /* ��֪a,b,A,B,��c,C */
{
float cosc,cosC;
cosc=(cos(a)*cos(b)-cos(A)*cos(B)*sin(a)*sin(b))/(1-sin(A)*sin(B)*sin(a)*sin(b));
if(cosc>=-1&&cosc<=1)  c=acos(cosc);
cosC=-cos(A)*cos(B)+sin(A)*sin(B)*cos(c);
if(cosC>=-1&&cosC<=1)  C=acos(cosC);
}

float trans(float xx) /*�ѽǶ�xת��Ϊ����*/
{
	return(xx*PI/180);
}

void out(float y,int *x) /*�ѽǶ�xת��Ϊ�ȷ�����ʽ*/
{ float xx;
	y=y/PI*180;
	*x=floor(y);
	xx=(y-*x)*60;
	*(x+1)=floor(xx);
	*(x+2)=floor((xx-*(x+1))*60);
}

void deg() /*ת������*/
{ char s;
  int d,f,m;
  float h;
do{	cprintf("��ѡ��:\n\r");
 	cprintf("1.�ȷ���--->����\n\r");
	cprintf("2.����--->����\n\r");
	cprintf("3.ʱ����--->����\n\r");
	cprintf("0.����\n\r");
	s=getch();
	cprintf("\n\r");
	if(s=='1') {cprintf("��"); scanf("%d",&d);
			  cprintf("��"); scanf("%d",&f);
			  cprintf("��"); scanf("%d",&m);
			  cprintf("����Ϊ%f\n\r",d+f/60.0+m/3600.0);}
	else if(s=='2') {cprintf("����Ϊ"); scanf("%f",&h);
				   cprintf("����Ϊ%f\n\r",h*180/PI);}
	else if(s=='3') {cprintf("ʱ(0-12)"); scanf("%d",&d);
				   cprintf("��(0-59)"); scanf("%d",&f);
				   cprintf("��(0-59)"); scanf("%d",&m);
				   cprintf("����Ϊ%f\n\r",(d+f/60.0+m/3600.0)*15);}
	cprintf("������(y/n)?\n\r");}
	while(getch()=='y');
}


void func1() /*��֪������*/
{ float tt;
input:   cprintf("�����������ߵ�ֵ��\n\r");
   cprintf("a="); scanf("%f",&a);
   cprintf("b="); scanf("%f",&b);
   cprintf("c="); scanf("%f",&c);
   if(a>0&&b>0&&c>0&&a<180&&b<180&&c<180)
    {  
       a=trans(a); b=trans(b); c=trans(c);  /*������Ķ���ת��Ϊ����*/
       tt=(cos(c)-cos(a)*cos(b))/(sin(a)*sin(b));
       if(tt>=-1&&tt<=1)  C=acos(tt);
       else return;
       getAB(); 
    }
    else 
    {
        cprintf("���벻��ȷ\n\r");
     goto input;
	}
}

void func2() /*��֪������*/
{ float tt;
input:	cprintf("�����������ǣ�\n\r");
		cprintf("A="); scanf("%f",&A);
		cprintf("B="); scanf("%f",&B);
		cprintf("C="); scanf("%f",&C);
  if(A>0&&A<180&&B>0&&B<180&&C<180&&C>0)
	{
        A=trans(A); B=trans(B); C=trans(C);  /*������Ķ���ת��Ϊ����*/
	tt=(cos(C)+cos(A)*cos(B))/(sin(A)*sin(B));
	if(tt>=-1&&tt<=1)  c=acos(tt);
	else return;
	getab();
	}
   else 
    {
        cprintf("���벻��ȷ\n\r");
     goto input;
	}
}

void func3() /*��֪���߼���н�*/
{ float tt;
input:	cprintf("����������:\n\r");
	cprintf("a="); scanf("%f",&a);
	cprintf("b="); scanf("%f",&b);
	cprintf("������нǣ�C="); scanf("%f",&C);
	if(a>0&&a<180&&b>0&&b<180&&C>0&&C<180)
	{
	a=trans(a); b=trans(b); C=trans(C);
	tt=cos(a)*cos(b)+sin(a)*sin(b)*cos(C);
	if(tt>=-1&&tt<=1) c=acos(tt);
	else return;
	getAB();
	}
	else 
    {
        cprintf("���벻��ȷ\n\r");
     goto input;
	}
}

void func4() /*��֪���Ǽ���б�*/
{ float tt;
input:	cprintf("����������:\n\r");
	cprintf("A="); scanf("%f",&A);
	cprintf("B="); scanf("%f",&B);
	cprintf("������б�:c="); scanf("%f",&c);
    if(A>0&&A<180&&B>0&&B<180&&c>0&&c<180)
	{A=trans(A); B=trans(B); c=trans(c);
	tt=-cos(A)*cos(B)+sin(A)*sin(B)*cos(c);
	if(tt>=-1&&tt<=1)  C=acos(tt);
	else return;
	getab();
	}
	else 
    {
        cprintf("���벻��ȷ\n\r");
     goto input;
	}
}

void func5() /*��֪���߼�һ�ߵĶԽ�*/
	{ float tt;
input:	cprintf("����һ�߼����ĶԽ�:��a="); scanf("%f",&a);
	cprintf("�Խ�A="); scanf("%f",&A);
	cprintf("������һ��:b="); scanf("%f",&b);
	if(a>0&&a<180&&A>0&&A<180&&b>0&&b<180)
	{ a=trans(a); A=trans(A); b=trans(b);
	tt=sin(A)*sin(c)/sin(a);
	if(tt>=-1&&tt<0)  B=asin(tt)+PI/2;
	else if(tt>=0&&tt<=1) B=asin(tt);
	else return;
	getCc();
	}
	else 
    {
        cprintf("���벻��ȷ\n\r");
     goto input;
	}
}

void func6()  /*��֪���Ǽ�һ�ǵĶԱ�*/
{ float tt;
input:	cprintf("����һ�Ǽ����ĶԱ�:��A=");  scanf("%f",&A);
	cprintf("�Ա�a="); scanf("%f",&a);
	cprintf("������һ����:B="); scanf("%f",&B);
	if(A>0&&A<180&&a>0&&a<180&&B>0&&B<180)
	{ A=trans(A); a=trans(a); B=trans(B);
	tt=sin(a)*sin(B)/sin(A);
	if(tt>=-1&&tt<0)  b=asin(tt)+PI/2;
	else if(tt>=0&&tt<=1) b=asin(tt);
	else return;
	getCc();
	}
	else 
    {
        cprintf("���벻��ȷ\n\r");
     goto input;
	}
}


void main()
{  char select;
int aa[3],bb[3],cc[3],AA[3],BB[3],CC[3];
  int *pa,*pb,*pc;
  int *pA,*pB,*pC;
  pa=aa;  pb=bb;   pc=cc;
  pA=AA;  pB=BB;   pC=CC;
start: do{
	window(1,1,80,8);
	textbackground(BLUE);
	textcolor(YELLOW);
	clrscr();
  A=B=C=a=b=c=0;
cprintf("----------------------------��ӭʹ���������Ǽ���С����--------------------------\n\r");
gotoxy(1,2);    cprintf("��ѡ����֪����������\n\r");
				cprintf("1.��֪������");
gotoxy(40,3);   cprintf("2.��֪������\n\r");
				cprintf("3.��֪���߼���н�");
gotoxy(40,4);	cprintf("4.��֪���Ǽ���н�\n\r");
				cprintf("5.��֪���߼�һ�ߵĶԽ�");
gotoxy(40,5);	cprintf("6.��֪���Ǽ�һ�ǵĶԱ�\n\r");
				cprintf("0.�˳�\n\r");
				cprintf("�����������ýǶ�(0--180)��ʾ����9ʹ�ýǶ�ת������");
 select=getch();
 cprintf("\n\r");
	window(1,9,80,25);
	textbackground(3);
	textcolor(5);
	clrscr();

if(select=='1')    /*��֪������*/
    func1();
else if(select=='2')    /*��֪������*/
	func2();
else if(select=='3')    /*��֪���߼���н�*/
	func3();
else if(select=='4')    /*��֪���Ǽ���б�*/
	func4();
else if(select=='5')    /*��֪���߼�һ�ߵĶԽ�*/
	func5();
else if(select=='6')    /*��֪���Ǽ�һ�ǵĶԱ�*/
	func6();
else if(select=='9') {deg(); goto start ;}
else goto loop;
if(a>0&&b>0&&c>0&&A>0&&B>0&&c>0)
{
	out(a,pa);   out(b,pb);   out(c,pc);
    out(A,pA);   out(B,pB);   out(C,pC);
	cprintf("�������ǵı߽Ƿֱ�Ϊ��\n\r");
	cprintf("a=%d��%d��%d��  b=%d��%d��%d��  c=%d��%d��%d��\n\r",aa[0],aa[1],aa[2],bb[0],bb[1],bb[2],cc[0],cc[1],cc[2]);
	cprintf("A=%d��%d��%d��  B=%d��%d��%d��  C=%d��%d��%d��\n\r",AA[0],AA[1],AA[2],BB[0],BB[1],BB[2],CC[0],CC[1],CC[2]);
}
else cprintf("�����������������һ������������!!!!!\n\r");
cprintf("Ҫ������(y/n)?  \n\r");
}

while(getch()=='y');

loop: cprintf("\n\r\n\r      �����ߣ�����   ��2000��6��\n\r");
       getch();
}
