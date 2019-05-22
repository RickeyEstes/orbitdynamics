// semi-major axis maneuver
#include "mandef.h"
using namespace Constant;

string orbitfilename, facfilename, attfilename;
 
double GLon, GLat, GAlt, Elmin = 5.0;
CFacility fac;

void loadfac(string filename)
{
	fstream file(filename, ios::in);
	if (!file.is_open())
		throw (string("Can't open file") + filename);
	std::string name, value;
	while (!file.eof()) {
		//# ����վλ����Ϣ
		//GLon = 120
		//GLat = 40
		//GAlt = 0.2
		if (ReadLine(&file, name, value)) {
			if (name == "GLon")
				sscanf(value.c_str(), "%lf", &GLon);
			else if (name == "GLat")
				sscanf(value.c_str(), "%lf", &GLat);
			else if (name == "GAlt")
				sscanf(value.c_str(), "%lf", &GAlt);
			else if (name == "Elmin")
				sscanf(value.c_str(), "%lf", &Elmin);
		}
	}
	fac.SetGeodetic(GLon, GLat, GAlt);
}

// ��������ָ������ʱ����̬�ǣ������жϿɼ���
void attctrl()
{
	CSatellite sat;
	InitSat(sat, orbitfilename);

	fstream fa(attfilename.c_str(), ios::out);
	vec3 satr,facr,sf,sfo,sfou;
	mat33 coi;
	double psi, theta;
	for (int i = 0; i <= 86400; i++)
	{
		satr = sat.Pos();
		facr = fac.ECIPos(sat.CurrentEpoch());
		sf = facr - satr;
		coi = GetCoi(sat.GetOrbitElements());
		sfo = coi * sf;
		sfou = sfo / norm(sfo, 2);
		
		// �жϿɼ���
		vec3 u = fac.ECFPosition;
		vec3 R = ECI_ECF(sat.CurrentEpoch())*sat.Pos();
		mat33 C = VehicleCoord(fac.Longitude, fac.Latitude);
		vec3 s = C * (R - u);
		double rou = norm(s, 2);
		double elevation = asin(s(2) / rou)*DEG;

		if (elevation > Elmin)
		{
			// 3-2ת����ת������
			psi = atan2(sfou(1), sfou(0));
			if (fabs(psi) < PI / 2)
				theta = asin(sfou(2));
			else
				theta = asin(-sfou(2));
			fa << sat.CurrentEpoch()+8*3600.0 << 
				TAB << psi * DEG << TAB << theta * DEG << TAB 
				<< sfou(0) << TAB << sfou(1) << TAB << sfou(2) << TAB << elevation << endl;
		}

		sat.Propagate(1, 1);
	}
	fa.close();
}

int main(int argc, char* argv[])
{
	if (argc<4)
	{
		printf("����δָ�������ļ�!!\n\n");
		printf("��ʹ������ָ����ã�\n");
		printf("  attctrl orbit.txt fac.txt attconangle.txt\n");
		printf("  ��һ���ļ�Ϊ��������ļ�\n");
		printf("  �ڶ����ļ�Ϊָ��ĵ�����ļ�\n");
		printf("  �������ļ�Ϊ�����̬�ļ�\n");
		return 0;
	}
	
	try {
		orbitfilename = string(argv[1]);
		facfilename = string(argv[2]);
		attfilename = string(argv[3]);

		loadfac(facfilename);
		attctrl();
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
	
	return 1;
}
