// semi-major axis maneuver
#include "mandef.h"
using namespace Constant;

string orbitfilename;
double ta;     // Ŀ�����볤��
CDateTime aepoch; // �볤�����ʱ��
double ti;     // Ŀ�����

// out:
CDateTime iepoch; // ��ǻ���ʱ��

//! ��Բ����İ볤�����
double sma_nearcircle()
{
	CSatellite sat;
	InitSat(sat, orbitfilename);
	sat.Propagate2Epoch(aepoch - 3600.0*8.0);
	Kepler km = Mean(sat.GetOrbitElements());
	double n = sqrt(GE / km.a / km.a / km.a);
	return n / 2 * (ta - km.a) * 1000; // m/s�ٶ�����
}

double i_raan()
{
	CSatellite sat;
	InitSat(sat, orbitfilename);
	if (sat.u > 0.5*PI && sat.u < 1.5*PI)
	{
		sat.Propagate2DescendingNode();
		iepoch = sat.CurrentEpoch();
		return sat.v * (ti * RAD - sat.i) * 1000;
	}
	else
	{
		sat.Propagate2AscendingNode();
		iepoch = sat.CurrentEpoch();
		return sat.v * (sat.i - ti * RAD) * 1000;
	}
}


void LoadLEOCAPFile(string filename)
{
	fstream file(filename, ios::in);
	if (!file.is_open())
		throw (string("Can't open file") + filename);
	std::string name, value;
	while (!file.eof()) {
		if (ReadLine(&file, name, value)) {
			if (name == "Targeta")
				sscanf(value.c_str(), "%lf", &ta);
			else if (name == "Targeti")
				sscanf(value.c_str(), "%lf", &ti);
			else if (name == "aepoch")
				aepoch = string2epoch(value);

		}
	}
}

int main(int argc, char* argv[])
{
	if (argc<4)
	{
		printf("����δָ�������ļ�!!\n\n");
		printf("��ʹ������ָ����ã�\n");
		printf("  leocap orbit.txt leocap.txt out.txt\n");
		printf("  ��һ���ļ�Ϊ��������ļ�\n");
		printf("  �ڶ����ļ�Ϊ����Ŀ�������ļ�\n");
		printf("  �������ļ�Ϊ����������ļ�\n");		
		return 0;
	}
	
	try {
		orbitfilename = string(argv[1]);

		LoadLEOCAPFile(string(argv[2]));		

		double dva = sma_nearcircle();
		double dvi = i_raan();

		fstream fp(string(argv[3]), ios::out);
		fp << " �볤����������ٶ����� = " << dva << endl;
		fp << " ��ǵ������ʱ�� = " << iepoch + 3600 * 8.0 << endl;
		fp << " ��ǵ����ٶ����� = " << dvi << endl;
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
