// semi-major axis maneuver
#include "mandef.h"
using namespace Constant;

string orbitfilename;

int main(int argc, char* argv[])
{
	if (argc<4)
	{
		printf("����δָ����������벻����!!\n\n");
		printf("��ʹ������ָ����ã�\n");
		printf("  geodeorbit geoorbit.txt 300 20 geodeout.txt\n");
		printf("  ��һ���ļ�Ϊ��������ļ�\n");
		printf("  �ڶ�������Ϊ̧�ߵĹ���߶ȣ���λΪkm\n");
		printf("  ����������Ϊ����������С����λΪN\n"); 
		printf("  ���ĸ��ļ�Ϊ����������ļ�\n");
		return 0;
	}
	
	try {
		orbitfilename = string(argv[1]);
		double dh = atof(argv[2]);
		double F = atof(argv[3]);

		CSatellite sat;
		InitSat(sat, orbitfilename);
		
		// ���Ƶ�Զ�ص����ص�
		double dt;
		if (sat.f < PI)
			dt = (PI - sat.f) / sat.n;
		else
			dt = (PI2 - sat.f) / sat.n;
		sat.Propagate(60, dt);
		CDateTime ep = sat.CurrentEpoch();

		double a1 = sat.a;
		double a2 = sat.a + dh;
		double at = ( a1 + a2 ) / 2;
		double dv1 = (sqrt(GE*(2 / a1 - 1 / at)) - sqrt(GE / a1)) * 1000;
		double dv2 = (sqrt(GE / a2) - sqrt(GE*(2 / a2 - 1 / at))) * 1000;

		fstream fp(string(argv[4]), ios::out);
		fp << "��һ�ο��ƿ���ʱ�� " << ep + 8 * 3600.0 << endl;
		fp << "��һ�ο��ƿ���ʱ�� " << dv1 * sat.Mass() / 20 << endl;
		fp << "��һ�ο����ٶ����� " << dv1 << endl;
		fp << "�ڶ��ο��ƿ���ʱ�� " << ep + 20 * 3600.0 << endl;
		fp << "�ڶ��ο��ƿ���ʱ�� " << dv2 * sat.Mass() / 20 << endl;
		fp << "�ڶ��ο����ٶ����� " << dv2 << endl;
		fp.close();

	}
	catch (BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
	catch (exception* e)
	{
		cerr << ((BaseException*)e)->what() << endl;
	}
	
	return 0;
}
