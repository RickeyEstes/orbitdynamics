#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

//! ���Թ������
void hohmantrans(double a1,double a2,double& dv1,double& dv2)
{
	double at = (a1 + a2) / 2;
	dv1 = sqrt(GE*(2 / a1 - 1 / at)) - sqrt(GE / a1);
	dv2 = sqrt(GE / a2) - sqrt(GE*(2 / a2 - 1 / at));
}

int main(int argc, char* argv[])
{	
	if (argc<3)
	{
		printf("���������������!!\n\n");
		printf("��ʹ������ָ����ã�\n");
		printf("  leotrans a1 a2 out.txt \n");
		printf("  ��һ������Ϊ��ʼ����볤��\n");
		printf("  �ڶ�������ΪĿ�����볤��\n");
		printf("  �������ļ���Ϊ����������ļ�\n");
		return 0;
	}

	double a1, a2;
	double dv1, dv2;
	sscanf(argv[1], "%lf", &a1);
	sscanf(argv[2], "%lf", &a2);
	hohmantrans(a1, a2, dv1, dv2);
	FILE* fp;
	fp = fopen(argv[3], "w");
	fprintf(fp, "dv1 = %lf\n", dv1 * 1000);
	fprintf(fp, "dv2 = %lf\n", dv2 * 1000);
	fclose(fp);

	return 0;
}