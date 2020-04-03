#include <OrbitDyn.h>
#include <PerfTimer.h>
#include "mandef.h"

using namespace Constant;

string outfilename, lanfilename, orbitfilename;
//#���������
double Lon = 120;
//#Զ�ص������
int Nap = 5;
//#��صĵ����ȷ�Χ
double minLon = 20;
double maxLon = 150;
//#���ǳ�ʼ����
double Mass0 = 5400;
//#������������
double F = 490;
//#���������ȳ�
double Isp = 315;

//! GEO���ת��
void geotrans()
{
	CSatellite sat;
	InitSat(sat, orbitfilename);

	//double hp = 200;
	//double ha = 35786;
	//double a = (hp + ha) / 2.0 + Re;
	//double e = (ha - hp) / 2.0 / a;
	//double i = 28.5 * RAD;
	//double w = 180 * RAD;
	//double lon0 = -9.2;
	double hp = sat.a*(1 - sat.e) - Re;
	double ha = sat.a*(1 + sat.e) - Re;
	double a = sat.a;
	double e = sat.e;
	double i = sat.i;
	double w = sat.w;
	double sg0 = GetSG(sat.CurrentEpoch());
	double lon0 = sat.Omega - sg0;
	int nrev[3] = { 1,2,1 };

	const double rs = 42164.2;
	double vav = sqrt(GE*(2 / rs - 1 / a));
	vec2 va;
	va(0) = vav * cos(i);
	va(1) = vav * sin(i);
	vec2 vs;
	vs(0) = sqrt(GE / rs);
	vs(1) = 0;
	vec2 dv = vs - va;
	double psidn = asin(-dv(1) / norm(dv, 2)) * DEG;
	double Londec0 = PI2 / sqrt(GE / a / a / a)*We*DEG;

	double x1 = 0.3;
	vec2 dv1 = dv * x1 * 1000.0;
	double dv1n = norm(dv1);
	double dm1 = Mass0 * (1 - exp( -dv1n / Isp / 9.8));
	double dt1 = dm1 / (F / Isp / 9.8);
	vec2 v1 = va + dv1 / 1000;
	double i1 = atan2(v1(1), v1(0));
	double v1n = norm(v1);
	double a1 = 1 / (2 / (ha + Re) - v1n * v1n / GE);
	double Londec1 = PI2 / sqrt(GE/a1/a1/a1)*We*DEG;
	double lon1 = mod(lon0 - Londec0 * (nrev[0] + 0.5), 360); // AF1 longitude
	double lon2 = mod(lon0 - Londec0 * (nrev[0] + 0.5) - Londec1 * nrev[1], 360); // AF2 longitude
	double dn = (Lon - lon2) / nrev[2];
	double a2m = pow(GE / pow( PI2 / ((360 - dn) / DEG / We),2), 1.0 / 3.0);
	double v2m = sqrt(GE*(2 / (ha + Re) - 1 / a2m));

	double anglev1dv = PI - acos(dot(v1,dv)/v1n/norm(dv,2));
	double theta = asin(sin(anglev1dv) / v2m * v1n);
	double dv2n = sqrt(v1n*v1n + v2m * v2m - 2.0*v2m*v1n*cos(PI - anglev1dv - theta));
	vec2 dv2 = dv2n * dv / norm(dv,2) * 1000;
	double dm2 = (Mass0 - dm1)*(1 - exp(-norm(dv2,2) / Isp / 9.8));
	double dt2 = dm2 / (F / Isp / 9.8);
	vec2 v2 = v1 + dv2 / 1000;
	double i2 = atan2(v2(1), v2(0));
	double a2 = 1 / (2.0 / (ha + Re) - norm(v2,2)*norm(v2, 2) / GE);
	double Londec2 = PI2 / sqrt(GE / a2 / a2 / a2)*We*DEG;

	vec2 dv3 = dv * 1000.0 - dv1 - dv2;
	double dm3 = (Mass0 - dm1 - dm2)*(1 - exp(-norm(dv3,2) / Isp / 9.8));
	double dt3 = dm3 / (F / Isp / 9.8);
	vec2 v3 = v2 + dv3 / 1000;
	double i3 = atan2(v3(1), v3(0));
	double a3 = 1 / (2 / (ha + Re) - norm(v3,2)*norm(v3, 2) / GE);
	double Londec3 = PI2 / sqrt(GE / a3 / a3 / a3)*We*DEG;

	double lontt = mod(lon0 - Londec0 * (nrev[0] + 0.5) - Londec1 * nrev[1] - Londec2 * nrev[2], 360); // final longitude
	cout << dv1.t();
	cout << dv2.t();
	cout << dv3.t();
	cout << lontt << endl;

	sat.SetAutoSave();
	CSatellite tmp;
	tmp.Initialize(sat.CurrentEpoch(),sat.GetOrbitElements());
	for(int kk = 0;kk< nrev[0];kk++)
		tmp.Propagate2Apogee();
	CDateTime EpAp = tmp.CurrentEpoch();
	//sat.Propagate2Apogee();
	//sat.Propagate2AscendingNode();
	//sat.Propagate2Apogee();
	//sat.PropagateBackward(60, dt1 / 2);
	sat.Propagate2Epoch(EpAp - dt1/2);
	
	fstream fout;
	fout.open(outfilename, ios::out);
	fout << "��1�ε��\n";
	fout << "����ʱ�� = " << sat.CurrentEpoch() << endl;
	fout << "�ػ�ʱ�� = " << sat.CurrentEpoch() + dt1 << endl;
	fout << "����ʱ��(s) = " << dt1 << endl;
	fout << "�ٶ�����(m/s) = " << norm(dv1, 2) << endl;
	fout << "�����ƫ����(��) = " << atan2(dv1(1),dv1(0))*DEG << endl;
	fout << "ȼ������(kg) = " << dm1 << endl;
	CSpherical lla = sat.GetLLA();
	fout << "����������� = " << lla.Longitude << endl;
	fout << "���������γ�� = " << lla.Latitude << endl;
	fout << "�������������� = " << sat.Mass() << endl;
	fout << "�������������� = " << sat.GetOrbitElements() << endl;

	sat.Propagate(60, dt1 / 2);
	vec3 dva;
	dva(0) = norm(dv1, 2) * cos(sat.i - atan2(dv1(1), dv1(0)));
	dva(1) = norm(dv1, 2) * sin(sat.i - atan2(dv1(1), dv1(0)));
	dva(2) = 0;
	fout << "�м���������� = " << sat.GetOrbitElements() << endl;
	sat.ImpluseManeuver(dva / 1000);
	sat.Mass0 = sat.Mass0*exp(-norm(dva,2) / Isp / 9.8);
	sat.Propagate(60, dt1 / 2);

	fout << "�ػ����������� = " << sat.GetOrbitElements() << endl;
	lla = sat.GetLLA();
	fout << "�ػ�������� = " << lla.Longitude << endl;
	fout << "�ػ������γ�� = " << lla.Latitude << endl;
	fout << "�ػ����������� = " << sat.Mass() << endl;

	fout << "\n";

	//sat.Propagate2Apogee();
	//sat.Propagate2Apogee();
	//sat.Propagate2AscendingNode();
	//sat.Propagate2AscendingNode();
	//sat.PropagateBackward(60, dt2 / 2);
	tmp.Initialize(sat.CurrentEpoch(), sat.GetOrbitElements());
	for (int kk = 0; kk < nrev[1]; kk++)
		tmp.Propagate2Apogee();
	EpAp = tmp.CurrentEpoch();
	sat.Propagate2Epoch(EpAp - dt2 / 2);

	fout << "��2�ε��\n";
	fout << "����ʱ�� = " << sat.CurrentEpoch() << endl;
	fout << "�ػ�ʱ�� = " << sat.CurrentEpoch() + dt2 << endl;
	fout << "����ʱ��(s) = " << dt2 << endl;
	fout << "�ٶ�����(m/s) = " << norm(dv2, 2) << endl;
	fout << "�����ƫ����(��) = " << atan2(dv2(1), dv2(0))*DEG << endl;
	fout << "ȼ������(kg) = " << dm2 << endl;
	lla = sat.GetLLA();
	fout << "����������� = " << lla.Longitude << endl;
	fout << "���������γ�� = " << lla.Latitude << endl;
	fout << "�������������� = " << sat.Mass() << endl;
	fout << "�������������� = " << sat.GetOrbitElements() << endl;

	sat.Propagate(60, dt2 / 2);
	dva(0) = norm(dv2, 2) * cos(sat.i - atan2(dv2(1), dv2(0)));
	dva(1) = norm(dv2, 2) * sin(sat.i - atan2(dv2(1), dv2(0)));
	dva(2) = 0;
	fout << "�м���������� = " << sat.GetOrbitElements() << endl; 
	sat.ImpluseManeuver(dva / 1000);
	sat.Mass0 = sat.Mass0*exp(-norm(dva, 2) / Isp / 9.8);
	sat.Propagate(60, dt2 / 2);
	fout << "�ػ����������� = " << sat.GetOrbitElements() << endl;
	lla = sat.GetLLA();
	fout << "�ػ�������� = " << lla.Longitude << endl;
	fout << "�ػ������γ�� = " << lla.Latitude << endl;
	fout << "�ػ����������� = " << sat.Mass() << endl;
	fout << "\n";

	tmp.Initialize(sat.CurrentEpoch(), sat.GetOrbitElements());
	for (int kk = 0; kk < nrev[2]; kk++)
		tmp.Propagate2Apogee();
	EpAp = tmp.CurrentEpoch();
	sat.Propagate2Epoch(EpAp - dt3 / 2);

	fout << "��3�ε��\n";
	fout << "����ʱ�� = " << sat.CurrentEpoch() << endl;
	fout << "�ػ�ʱ�� = " << sat.CurrentEpoch() + dt3 << endl;
	fout << "����ʱ��(s) = " << dt3 << endl;
	fout << "�ٶ�����(m/s) = " << norm(dv3, 2) << endl;
	fout << "�����ƫ����(��) = " << atan2(dv3(1), dv3(0))*DEG << endl;
	fout << "ȼ������(kg) = " << dm3 << endl;
	lla = sat.GetLLA();
	fout << "����������� = " << lla.Longitude << endl;
	fout << "���������γ�� = " << lla.Latitude << endl;
	fout << "�������������� = " << sat.Mass() << endl;
	fout << "�������������� = " << sat.GetOrbitElements() << endl;

	sat.Propagate(60, dt3 / 2);
	dva(0) = norm(dv3, 2) * cos(sat.i - atan2(dv3(1), dv3(0)));
	dva(1) = norm(dv3, 2) * sin(sat.i - atan2(dv3(1), dv3(0)));
	dva(2) = 0;
	fout << "�м���������� = " << sat.GetOrbitElements() << endl; 
	sat.ImpluseManeuver(dva*1.02 / 1000);
	sat.Mass0 = sat.Mass0*exp(-norm(dva, 2) / Isp / 9.8);
	fout << "�ػ����������� = " << sat.GetOrbitElements() << endl;
	sat.Propagate(60, dt3 / 2);
	lla = sat.GetLLA();
	fout << "�ػ�������� = " << lla.Longitude << endl;
	fout << "�ػ������γ�� = " << lla.Latitude << endl;
	fout << "�ػ����������� = " << sat.Mass() << endl;
	fout << "\n";

	fout << "ĩ�˹������ = " << sat.GetOrbitElements() << endl;

	fout << "���۹ػ�������� = " << lontt << endl;

	fout.close();
}

void loadcon(string lanfilename) {
	fstream flan(lanfilename, ios::in);
	string name, value;
	while (!flan.eof()) {
		//#���������
		//Lon = 120
		//#Զ�ص������
		//Nap = 5
		//#��صĵ����ȷ�Χ
		//minLon = 20
		//maxLon = 150
		//#������������
		//F = 490
		//#���������ȳ�
		//Isp = 315
		if (ReadLine(&flan, name, value)) {
			if (name == "Lon")
				sscanf(value.c_str(), "%lf", &Lon);
			else if (name == "Nap")
				sscanf(value.c_str(), "%d", &Nap);
			else if (name == "F")
				sscanf(value.c_str(), "%lf", &F);
			else if (name == "Isp")
				sscanf(value.c_str(), "%lf", &Isp);
			else if (name == "minLon")
				sscanf(value.c_str(), "%lf", &minLon);
			else if (name == "maxLon")
				sscanf(value.c_str(), "%lf", &maxLon);
			else if (name == "Mass0")
				sscanf(value.c_str(), "%lf", &Mass0);
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		printf("����δָ�������ļ�!!\n\n");
		printf("��ʹ������ָ����ã�\n");
		printf("  geotrans gto.txt geotranscfg.txt geotransout.txt\n");
		printf("  ��һ���ļ�Ϊ��������ļ�\n");
		printf("  �ڶ����ļ�Ϊͬ�����ת�������ļ�\n");
		printf("  �������ļ�Ϊͬ�����ת�ƿ�������ļ�\n");
		return 0;
	}

	try {
		orbitfilename = string(argv[1]);
		lanfilename = string(argv[2]);
		outfilename = string(argv[3]);

		loadcon(lanfilename);
		geotrans();
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