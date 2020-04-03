#include <OrbitDyn.h>
#include <ConsoleUtility.h>
#include "mandef.h"

using namespace Constant;

string outfilename, lanfilename, orbitfilename;
CDateTime epoch;
vector<Kepler> kplist;
vector<string> satname;
typedef struct {
	int SatPlaneNo;  // ���ǹ������
	int SatInplaneNo; // ���ǹ�����ڱ��
	double Omega;  // ���Ǳ��������ྭ
	double u;      // ���Ǳ�ƹ������
}cspos;
vector<cspos> csp;

int N = 0, P = 0, F = 0;
double a0, i0, Omega0, u0, du, abias;

//! ����λ�ñ���
void consform()
{
	string name, value;
	fstream fo(orbitfilename, ios::in);
	while (!fo.eof()) {
		//Epoch = 2019 - 05 - 01 00:00 : 00
		//Sat11 = 7478.0   0.00   85.0  10.000   0.0   0.0000000
		//Sat12 = 7478.0   0.00   85.0  10.000   0.0   45.0000000
		if (ReadLine(&fo, name, value)) {
			if (name == "Epoch")
			{
				epoch = string2epoch(value);
				cout << epoch << endl;
			}
			else
			{
				satname.push_back(name);
				stringstream ss(value);
				Kepler kp;
				ss >> kp;
				kp.i = kp.i*RAD;
				kp.o = kp.o*RAD;
				kp.w = kp.w*RAD;
				kp.M = kp.M*RAD;
				kplist.push_back(kp);
				cout << name << TAB << kp << TAB << Mean(kp) << endl;
			}
		}
	}
	fo.close();

	int nsat = kplist.size();
	CSatellite* cs = new CSatellite[nsat];
	int nm, id;
	for ( id = 0; id < nsat; id++)
	{
		//CSatellite s;
		cs[id].Initialize(epoch, kplist[id]);
		cs[id].Name = satname[id];
	}
	// average orbit angle velocity
	double n = sqrt(GE / a0 / a0 / a0);
	// node period angle velocity
	double w = n / (1 + 1.5*J2*(Re / a0)*(Re / a0)*(1 - 4.0*pow(cos(i0*RAD), 4)));

	ConsoleUtility cu;
	bool oneover = false;
	int overid = -1;
	double step = 120;
	double printp = floor(86400 / step);
	for (nm = 0; nm < 86400*30/step; nm++)
	{
		if (mod(nm, 50) == 0) {
			cu.Rollto(nsat + 1);
			cout << cs[0].CurrentEpoch() + 8 * 3600 << "                           " << endl;
		}
		for (id = 0; id < nsat; id++)
		{
			double u = cs[id].u;
			double stdu = fmod(csp[id].u*RAD + w * nm * step, PI2);
			double ue = u - stdu;
			if (ue > PI)
				ue -= PI2;
			else if (ue < -PI)
				ue += PI2;
			if ( fabs(ue) > du*RAD /*&& oneover==false*/)
			{
				//Kepler km = Mean(cs[id].GetOrbitElements());
				//double dlta = a0 - km.a;
				//double dv = 0.5 * n * (dlta + abias / 1000.0*sign(dlta)) * 1000.0;
				//vec3 v;
				//v(0) = dv / 1000.0;
				//v(1) = 0;
				//v(2) = 0;
				//cs[id].ImpluseManeuver(v);
				oneover = true;
				overid = id;
			}

			cs[id].Propagate(step, step);
			
			if(mod(nm, 50)==0)
				printf("%s.ue = %6.2f stdu = %6.2f u = %6.2f           \n",
					satname[id].c_str(), ue*DEG, stdu*DEG, u*DEG);
		}

		if (oneover)
			break;
	}
	if(oneover)
		cout << cs[overid].CurrentEpoch()+8*3600 << " ����" << satname[overid] << "�����߽�" << endl;

	Kepler km = Mean(cs[overid].GetOrbitElements());
	double dlta = a0 - km.a;
	double dv = 0.5 * n * (dlta + abias/1000.0*sign(dlta)) * 1000.0;

	fstream fout(outfilename, ios::out);
	fout << "time = " << cs[overid].CurrentEpoch() + 8 * 3600 << endl;
	fout << "sat ID =  " << satname[overid] << endl;
	fout << "dv = " << dv << endl;
	fout.close();
	
	// test
	vec3 v;
	v(0) = dv/1000.0;
	v(1) = 0;
	v(2) = 0;
	cs[overid].ImpluseManeuver(v);
	int te;
	for (te = 0; te < 86400 * 10 / step; te++)
	{
		if (mod(te, 50) == 0) {
			cu.Rollto(nsat*2 + 2);
			cout << cs[overid].CurrentEpoch() + 8 * 3600 << "                           " << endl;
		}

		double u = cs[overid].u;
		double stdu = fmod(csp[overid].u*RAD + w * nm * step + w*te*step, PI2);
		double ue = u - stdu;
		if (ue > PI)
			ue -= PI2;
		else if (ue < -PI)
			ue += PI2;
		//if (fabs(ue) > du*RAD)
		//{			
		//	break;
		//}

		cs[overid].Propagate(step, step);

		if (mod(te, 50) == 0)
			printf("%s.ue = %6.2f stdu = %6.2f u = %6.2f           \n",
				satname[overid].c_str(), ue*DEG, stdu*DEG, u*DEG);
	
	}


	//delete[] cs;
	//delete cs;
}

//Walker�����Ĺ�����Ϊ��N/P/F��������Ŀ/���ƽ����/��λ����)
void Walker(int N, int P, int F,double Omega0,double u0)
{
	int S = N / P;
	FILE *fp;
	fp = fopen("walker.txt", "w");
	double Omega, u;
	for (int Pm = 1; Pm <= P; Pm++)
	{
		Omega = Omega0 + 360 / P * (Pm - 1);
		for (int Nm = 1; Nm <= S; Nm++)
		{
			u = u0 + 360 / S * (Nm - 1) + 360 / N * F*(Pm - 1);
			fprintf(fp, "%lf   %lf\n", Omega, u);
			cspos c;
			c.SatPlaneNo = Pm;
			c.SatInplaneNo = Nm;
			c.Omega = Omega;
			c.u = u;
			csp.push_back(c);
		}
	}
	fclose(fp);
}

void loadcon(string lanfilename) {
	fstream flan(lanfilename, ios::in);
	double lon;
	string name, value;
	while (!flan.eof()) {
		//# ������Ŀ
		//N = 24
		//# ���ƽ����
		//P = 3
		//# ��λ����
		//F = 2
		//# ��һ��������������ྭ
		//Omega0 = 10
		//# ��һ���ǵ�γ�ȷ���
		//u0 = 0
		if (ReadLine(&flan, name, value)) {
			if (name == "N")
				sscanf(value.c_str(), "%d", &N);
			else if (name == "P")
				sscanf(value.c_str(), "%d", &P);
			else if (name == "F")
				sscanf(value.c_str(), "%d", &F); 
			else if (name == "Omega0")
				sscanf(value.c_str(), "%lf", &Omega0);
			else if (name == "u0")
				sscanf(value.c_str(), "%lf", &u0);
			else if (name == "a0")
				sscanf(value.c_str(), "%lf", &a0);
			else if (name == "i0")
				sscanf(value.c_str(), "%lf", &i0);
			else if (name == "du")
				sscanf(value.c_str(), "%lf", &du);
			else if (name == "abias")
				sscanf(value.c_str(), "%lf", &abias);
		}
	}
	if (N != 0 && P != 0)
	{
		Walker(N,P,F,Omega0,u0);
	}
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		if (argc == 2)
		{
			loadcon(argv[1]);
			return 1;
		}

		printf("����δָ�������ļ�!!\n\n");
		printf("��ʹ������ָ����ã�\n");
		printf("  consformation csorbit.txt conform.txt csout.txt\n");
		printf("  ��һ���ļ�Ϊ��������ļ�\n");
		printf("  �ڶ����ļ�Ϊ���������������ļ�\n");
		printf("  �������ļ�Ϊ���ά�־��������ļ�\n");
		printf("  ���ĸ��ļ�Ϊ���ά�ֿ�������ļ�\n");
		printf(" �÷�2�� ����һ�������ı��������ྭ��γ�ȷ��Ǳ�\n");
		printf(" consformation conform.txt\n");
		printf(" �����ļ��еĻع�������������һ��walker.txt�ļ����������Ȧ�εı�������������\n");
		return 0;
	}

	try {
		orbitfilename = string(argv[1]);
		lanfilename = string(argv[2]);
		outfilename = string(argv[3]);

		loadcon(lanfilename);
		consform();
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