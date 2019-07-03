#include <OrbitDyn.h>
#include "mandef.h"

using namespace Constant;

string outfilename, lanfilename, orbitfilename;
double  Ti,To,Laterr, F;

Kepler ECIkp2ECFkp(CDateTime ep, vec3 p, vec3 v)
{
	mat33 cfi = ECI_ECF(ep);
	vec3 pf, vf;
	pf = cfi * p;
	vf = cfi * v;
	return Cartesian_Kepler(pf, vf);
}

void geoEWsk()
{
	CSatellite sat;
	InitSat(sat, orbitfilename);

	// �ع�ϵ�������
	Kepler kf;
	// �ع�ϵ���ʸ��
	double ix, iy;

	//  Ԥ���Ϳ��Ƽ���
	fstream fp;
	fp.open("NSsktraj.txt", ios::out);
	double di;
	int i = 0;
	double itx, ity, ixe, iye;
	bool nssk = true;
	do {
		sat.Propagate(600, 600);
		kf = ECIkp2ECFkp(sat.CurrentEpoch(), sat.Pos(), sat.Vel());

		ix = kf.i * DEG * cos(kf.o);
		iy = kf.i * DEG * sin(kf.o);
		itx = Ti * cos(To*RAD);
		ity = Ti * sin(To*RAD);
		ixe = ix - itx;
		iye = iy - ity;
		di = sqrt(ixe * ixe + iye * iye);

		if (mod(i++, 144) == 0) {
			cout << sat.CurrentEpoch() + 8 * 3600.0 << "\t���:" << kf.i*DEG
				<< "\t���ʸ��X:" << ix
				<< "\t���ʸ��Y:" << iy << endl;
		}

		// control
		if (di > Laterr)
		{
			double T = (sat.CurrentEpoch().GetMJD() - MJD2000) / 36525;
			double orb_Omega_l = 2.182439f - 33.757f*T;
			double Omegaix = -3.5e-4*sin(orb_Omega_l);
			double Omegaiy = 22.79e-4 + 2.59e-4*cos(orb_Omega_l);
			double Omegat = atan2(Omegaiy, Omegaix) + PI;
			double it = Laterr;
			double dix = ixe - it * cos(Omegat);
			double diy = iye - it * sin(Omegat);
			double dVy = 3074.6*sqrt(dix*dix + diy * diy)*RAD;
			double NSFireDuration = dVy * sat.Mass() / F;

			double dalf = atan2(diy, dix) - mod(sat.u + sat.Omega + 4 * PI, 2 * PI);
			if (dalf < 0)
				dalf += PI2;
			int autoSKbTDS;
			if (dalf < 2.3562 || dalf > 5.4978)
			{
				autoSKbTDS = 0xF0;
			}
			else {
				dalf += PI;
				if (dalf > 2 * PI)
					dalf -= 2 * PI;
				autoSKbTDS = 0x00;
			}
			CDateTime NSTime = sat.CurrentEpoch() + dalf / We - NSFireDuration / 2;

			fstream fout;
			fout.open(outfilename, ios::out);
			fout << "����ʱ�� = " << NSTime << endl;
			fout << "����ʱ��(s) = " << NSFireDuration << endl;
			fout << "�ٶ�����(m/s) = " << dVy << endl;
			fout << "λ������ = ";
			if (autoSKbTDS == 0xF0)
			{
				fout << "��λ��" << endl;
			}
			else
			{
				fout << "����λ��" << endl;
			}
			fout << "λ��ǰ���(��) = " << kf.i << endl;
			fout << "λ��ǰ������ྭ(��) = " << kf.o*DEG << endl;

			double ifx = Ti * cos(To*RAD) + it * cos(Omegat);
			double ify = Ti * sin(To*RAD) + it * sin(Omegat);

			fout << "λ�������(��) = " << sqrt(ifx*ifx+ify*ify) << endl;
			fout << "λ����������ྭ(��) = " << atan2(ify,ifx) * DEG << endl;
			fout.close();

			nssk = false;
			// �����߽�
		}
		fp << sat.CurrentEpoch() + 8 * 3600.0 << TAB << ix << TAB << iy << endl;
	} while (nssk);
	fp.close();
}

void loadNSsk(string lanfilename) {
	fstream flan(lanfilename, ios::in);
	string name, value;
	while (!flan.eof()) {
		//#Ŀ�����
		//Ti = 0.3
		//#Ŀ��������ྭ
		//To = 40
		//# ����γ�ȱ��ַ�Χ
		//Laterr = 0.1
		//# �ϱ�λ������
		//F = 19
		if (ReadLine(&flan, name, value)) {
			if (name == "Laterr")
				sscanf(value.c_str(), "%lf", &Laterr);
			else if (name == "F")
				sscanf(value.c_str(), "%lf", &F);
			else if (name == "Ti")
				sscanf(value.c_str(), "%lf", &Ti);
			else if (name == "To")
				sscanf(value.c_str(), "%lf", &To);
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		printf("����δָ�������ļ�!!\n\n");
		printf("��ʹ������ָ����ã�\n");
		printf("  geocolocationI geoorbit.txt NScosk.txt geocosk.txt\n");
		printf("  ��һ���ļ�Ϊ��������ļ�\n");
		printf("  �ڶ����ļ�Ϊ�ϱ�λ�������ļ�\n");
		printf("  �������ļ�Ϊ�ϱ�λ����������ļ�\n");
		printf("  ���н����������һ��NSsktraj.txt�ļ������滭���ߵ����ݣ�����ʱ�䡢���ʸ��x�����ʸ��y\n");
		return 0;
	}

	try {
		orbitfilename = string(argv[1]);
		lanfilename = string(argv[2]);
		outfilename = string(argv[3]);

		loadNSsk(lanfilename);
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