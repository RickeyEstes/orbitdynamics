#include "stdio.h"
#include "process.h"

void main()
{
	// ʱ��(����ʱ)
	char* Epoch = "2019-01-01 00:00:00";
	// ���ص�߶�(km)
	double Hp  =   17120;
	// Զ�ص�߶�(km)
	double Ha  =   35786;
	// ���(��)
	double incl  =  4.8;
	// ��ʼ����(kg)
	double mass0 = 1600;
	// �������(N)
	double f = 0.48;
	// �ȳ�(s)
	double isp = 2450;
	// ������ྭ(��)
	double Omega = 0;
	// ���ص����(��)
	double w = 180;
	// ƽ�����(��)
	double M = 0;

	// д�����ļ�
	FILE* fp = fopen("in.txt","w");
	fprintf(fp,"Epoch = %s\n",Epoch);
	fprintf(fp,"Hp = %lf\n",Hp);
	fprintf(fp,"Ha = %lf\n",Ha);
	fprintf(fp,"incl = %lf\n",incl);
	fprintf(fp,"f = %lf\n",f);
	fprintf(fp,"isp = %lf\n",isp);
	fprintf(fp,"Omega = %lf\n",Omega);
	fprintf(fp,"w = %lf\n",w);
	fprintf(fp,"M = %lf\n",M);
	fclose(fp);

	// �����Ż�����
	printf("start lt.exe ...\n");
	//_execl("lt.exe","lt.exe","in.txt");
	system("lt in.txt");

	//��ȡ������
	FILE* result = fopen("transparam.txt","r");
	double tof,dm,t1,t2,t3,dm1,dm2,dm3,PHI1,PHI2;
	char* c1 = "cccccccc";
	char* c2 = "=";
	fscanf(result,"%s %s %lf",&c1,&c2,&PHI1);
	fscanf(result,"%s %s %lf",&c1,&c2,&PHI2);
	fscanf(result,"%s %s %lf",&c1,&c2,&tof);
	fscanf(result,"%s %s %lf",&c1,&c2,&dm);
	fscanf(result,"%s %s %lf",&c1,&c2,&t1);
	fscanf(result,"%s %s %lf",&c1,&c2,&t2);
	fscanf(result,"%s %s %lf",&c1,&c2,&t3);
	fscanf(result,"%s %s %lf",&c1,&c2,&dm1);
	fscanf(result,"%s %s %lf",&c1,&c2,&dm2);
	fscanf(result,"%s %s %lf",&c1,&c2,&dm3);
	fclose(result);
}