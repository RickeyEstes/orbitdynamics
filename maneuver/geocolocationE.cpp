#include <OrbitDyn.h>
#include "mandef.h"

using namespace Constant;

string outfilename, lanfilename, orbitfilename;
double etx, ety, ee, F;

void geoEWsk()
{
	CSatellite sat;
	InitSat(sat, orbitfilename);

	CDateTime ep0 = sat.CurrentEpoch();
	double Vs = sqrt(GE / 42164.2);

	fstream fp;
	fp.open("EWcosktraj.txt", ios::out);
	double ex, ey;
	bool checke = true;
	do {
		sat.Propagate(600, 600);

		ex = sat.e*cos(sat.Omega + sat.w);
		ey = sat.e*sin(sat.Omega + sat.w);

		double eerrx = ex - etx;
		double eerry = ey - ety;
		double eerr = sqrt(eerrx*eerrx + eerry * eerry);

		// TODO: control
		if (eerr > ee)
		{
			double lsat = sat.Omega + sat.w + sat.f;
			while (lsat > PI2) lsat -= PI2;
			double dv1 = 0.25*Vs*eerr*1000;
			double duration1 = abs(dv1) * sat.Mass() / F;
			double lb = atan2(eerry, eerrx);
			double delta_l = lb - lsat;
			while (delta_l < 0) delta_l += PI2;
			CDateTime t1 = sat.CurrentEpoch() + delta_l / We;

			double dv2 = -dv1;
			CDateTime t2 = t1 + 43200.0;
			double duration2 = abs(dv2) * sat.Mass() / F;


			fstream fout;
			fout.open(outfilename, ios::out);
			fout << "��һ�ο��ƿ���ʱ�� = " << t1 + 8*3600.0 << endl;
			fout << "��һ�ο��ƿ���ʱ�� = " << duration1 << endl;
			fout << "��һ�ο����ٶ����� = " << dv1 << endl;
			fout << "�ڶ��ο��ƿ���ʱ�� = " << t2 + 8 * 3600.0 << endl;
			fout << "�ڶ��ο��ƿ���ʱ�� = " << duration2 << endl;
			fout << "�ڶ��ο����ٶ����� = " << dv2 << endl;
			fout.close();

			checke = false;
		}
		fp << sat.CurrentEpoch() + 8 * 3600.0 << TAB << ex << TAB << ey << endl;
	} while (checke);
	fp.close();
}

void loadlon(string lanfilename) {
	fstream flan(lanfilename, ios::in);
	string name, value;
	while (!flan.eof()) {
		//#Ŀ��ƫ����ʸ��X����
		//ex = 0.00005
		//#Ŀ��ƫ����ʸ��Y����
		//ey = -0.00005
		//# ����λ������
		//F = 18
		if (ReadLine(&flan, name, value)) {
			if (name == "ex")
				sscanf(value.c_str(), "%lf", &etx);
			else if (name == "ey")
				sscanf(value.c_str(), "%lf", &ety);
			else if (name == "ee")
				sscanf(value.c_str(), "%lf", &ee);
			else if (name == "F")
				sscanf(value.c_str(), "%lf", &F);
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		printf("����δָ�������ļ�!!\n\n");
		printf("��ʹ������ָ����ã�\n");
		printf("  geocolocationE geoorbit.txt EWcosk.txt geosk.txt\n");
		printf("  ��һ���ļ�Ϊ��������ļ�\n");
		printf("  �ڶ����ļ�Ϊ������λƫ���ʲ��������ļ�\n");
		printf("  �������ļ�Ϊ����λ����������ļ�\n");
		return 0;
	}

	try {
		orbitfilename = string(argv[1]);
		lanfilename = string(argv[2]);
		outfilename = string(argv[3]);

		loadlon(lanfilename);
		geoEWsk();
	}
	catch (BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
	catch (exception* e)
	{
		cerr << ((BaseException*)e)->what() << endl;
	}
	catch (string * e)
	{
		cerr << *e << endl;
	}
	return 0;
}