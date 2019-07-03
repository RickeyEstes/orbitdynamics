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
		printf("  leodeorbit orbit.txt 12.34 out.txt\n");
		printf("  ��һ���ļ�Ϊ��������ļ�\n");
		printf("  �ڶ�������Ϊ����ʣ����ٶ���������λΪm/s\n");
		printf("  �������ļ�Ϊ����������ļ�\n");		
		return 0;
	}
	
	try {
		orbitfilename = string(argv[1]);
		double dv = atof(argv[2]);		

		CSatellite sat;
		InitSat(sat, orbitfilename);
		
		// �������Բ��������Ƶ�Զ�ص�
		if(sat.e>0.0001)
			sat.Propagate2Apogee();
		CDateTime ep = sat.CurrentEpoch();

		// ����Զ�ص��ٶ�
		double va = sat.v - dv/1000.0;
		// Զ�ص�߶�
		double ra = sat.r;
		// ���� v^2/miu = 2/r - 1/a ����볤��
		double a = 1 / (2 / ra - va * va / GE);
		double hp = (a*2.0 - ra) - Re;
		
		fstream fp(string(argv[3]), ios::out);
		fp << " ���ص�߶� = " << hp << endl;
		fp << " Զ�ص�߶� = " << ra - Re << endl;
		fp << " Զ�ص�ʱ�̿� = " << ep + 3600*8.0 << endl;		
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
