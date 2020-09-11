// semi-major axis maneuver
#include "mandef.h"
using namespace Constant;

string orbitfilename, ssinstallfilename, tmfilename, attfilename;
 

// ������װ����
mat33 ssa, ssb, ssc;

void loadssinstall(string filename)
{
	fstream fs(filename, ios::in);
	if (!fs.is_open())
	{
		throw string("Can't open file ")+filename.c_str();
	}
	fs >> ssa(0, 0) >> ssa(0, 1) >> ssa(0, 2)
		>> ssa(1, 0) >> ssa(1, 1) >> ssa(1, 2)
		>> ssa(2, 0) >> ssa(2, 1) >> ssa(2, 2);
	fs >> ssb(0, 0) >> ssb(0, 1) >> ssb(0, 2)
		>> ssb(1, 0) >> ssb(1, 1) >> ssb(1, 2)
		>> ssb(2, 0) >> ssb(2, 1) >> ssb(2, 2);
	fs >> ssc(0, 0) >> ssc(0, 1) >> ssc(0, 2)
		>> ssc(1, 0) >> ssc(1, 1) >> ssc(1, 2)
		>> ssc(2, 0) >> ssc(2, 1) >> ssc(2, 2);

	cout << "ssa = \n" << ssa << endl;
	cout << "ssb = \n" << ssb << endl;
	cout << "ssc = \n" << ssc << endl;
}

// ���ɲ����õ�ң������
void maketmdata()
{
	CSatellite sat;
	InitSat(sat, orbitfilename);
	cout << sat.CurrentEpoch() << TAB << sat.GetOrbitElements() << endl;

	mat33 coi;
	fstream fs("tm.txt", ios::out);
	fs.precision(9);
	CEuler eu(1.0*RAD,-0.2*RAD,0.5*RAD);
	mat33 cbo = eu.ToMatrix();
	for (double t = 0;t < 6000;t++)
	{
		coi = GetCoi(sat.GetOrbitElements());

		eu.phi = WhiteNoise(0.002*RAD, 1 * RAD);
		eu.theta = WhiteNoise(0.002*RAD, -0.2*RAD);
		eu.psi = WhiteNoise(0.002*RAD, 0.5*RAD);
		cbo = eu.ToMatrix();		
		CQuaternion qa(ssa*cbo*coi);

		eu.phi = WhiteNoise(0.002*RAD, 1 * RAD);
		eu.theta = WhiteNoise(0.002*RAD, -0.2*RAD);
		eu.psi = WhiteNoise(0.002*RAD, 0.5*RAD);
		cbo = eu.ToMatrix();
		CQuaternion qb(ssb*cbo*coi);

		eu.phi = WhiteNoise(0.002*RAD, 1 * RAD);
		eu.theta = WhiteNoise(0.002*RAD, -0.2*RAD);
		eu.psi = WhiteNoise(0.002*RAD, 0.5*RAD);
		cbo = eu.ToMatrix();
		CQuaternion qc(ssc*cbo*coi);
		fs << sat.CurrentEpoch() << TAB << qa 
			<< TAB << qb << TAB << qc << endl;
		sat.Propagate(1, 1);
	}
	fs.close();
}

void ssm2euler(CQuaternion q, mat33 ss, mat33 coi, CEuler& att)
{
	mat33 csi = q.C();
	mat33 cbo = ss.t()*csi*coi.t();
	att.SetValueFromMatrix312(cbo);
}

void attdet()
{
	CSatellite sat;
	InitSat(sat, orbitfilename);

	// ��ȡң�����ݣ�ȷ����̬�������
	fstream fs(tmfilename, ios::in);
	fstream fatt(attfilename, ios::out);
	string date, time;
	CQuaternion qa, qb, qc;
	CDateTime t;
	CEuler att;
	while (!fs.eof()) {
		fs >> date >> time;
		fs >> qa.qx >> qa.qy >> qa.qz >> qa.qs;
		fs >> qb.qx >> qb.qy >> qb.qz >> qb.qs;
		fs >> qc.qx >> qc.qy >> qc.qz >> qc.qs;
		t = string2epoch(date + " " + time);
		sat.Propagate2Epoch(t);
		mat33 coi = GetCoi(sat.GetOrbitElements());
		fatt << date << " " << time << TAB;
		ssm2euler(qa, ssa, coi, att);
		fatt << att.phi*DEG << TAB << att.theta*DEG << TAB << att.psi*DEG << TAB;
		ssm2euler(qb, ssb, coi, att);
		fatt << att.phi*DEG << TAB << att.theta*DEG << TAB << att.psi*DEG << TAB;
		ssm2euler(qc, ssc, coi, att);
		fatt << att.phi*DEG << TAB << att.theta*DEG << TAB << att.psi*DEG << endl;
	}
	fs.close();
	fatt.close();
}


int main(int argc, char* argv[])
{
	if (argc<5)
	{
		// ģ��ң������
		if (argc == 3)
		{
			orbitfilename = string(argv[1]);
			ssinstallfilename = string(argv[2]);
			loadssinstall(ssinstallfilename);
			printf("making attdet tm.txt test data....\n");
			maketmdata();
			return 1;
		}
		// ��ʾ
		printf("����δָ�������ļ�!!\n\n");
		printf("��ʹ������ָ����ã�\n");
		printf("  attdet orbit.txt ssinstall.txt tm.txt att.txt\n");
		printf("  ��һ���ļ�Ϊ��������ļ�\n");
		printf("  �ڶ����ļ�Ϊ���������ݰ�װ�ļ�\n");
		printf("  �������ļ�Ϊң�������ļ�\n");
		printf("  ���ĸ��ļ�Ϊ�����̬�ļ�\n");
		return 0;
	}
	
	try {
		orbitfilename = string(argv[1]);
		ssinstallfilename = string(argv[2]);
		tmfilename = string(argv[3]);
		attfilename = string(argv[4]);

		loadssinstall(ssinstallfilename);
		attdet();

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
