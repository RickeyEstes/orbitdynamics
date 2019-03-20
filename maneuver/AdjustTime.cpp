// adjusttime.cpp : �������̨Ӧ�ó������ڵ㡣

// satellite time adjust

#include "OrbitDyn.h"
#include "mandef.h"

using namespace Constant;

CFacility fac(120,40,0.2);

OrbitParam op;

double delaysat;
double delaygrd;
double GLon,GLat,GAlt;

typedef struct {
	CDateTime ts;
	CDateTime re;
	double dt;
} timepair;
vector<timepair> tl;

void LoadFacFile(string filename)
{
	fstream file(filename,ios::in);
	if(!file.is_open())
		throw (string("Can't open file") + filename);
	std::string name,value;
	while(!file.eof()){
		//#����ʱ��
		//Satdelay  = 100
		//#����ʱ��
		//Grounddelay = 100
		//# ����վλ����Ϣ
		//GLon = 120
		//GLat = 40
		//GAlt = 0.2
		if( ReadLine(&file,name,value) ) {
			if(name == "Satdelay")
			{
				sscanf(value.c_str(),"%lf",&delaysat);
				delaysat /= 1000;
			}
			else if(name == "Grounddelay")
			{
				sscanf(value.c_str(),"%lf",&delaygrd);
				delaygrd /= 1000;
			}
			else if(name == "GLon")
				sscanf(value.c_str(),"%lf",&GLon);
			else if(name == "GLat")
				sscanf(value.c_str(),"%lf",&GLat);
			else if(name == "GAlt")
				sscanf(value.c_str(),"%lf",&GAlt);
		}
	}
	fac.SetGeodetic(GLon, GLat, GAlt);
}

void LoadTimeFile(string filename)
{
	fstream fdat(filename,ios::in);
	if(!fdat.is_open())
		throw string("Can't open adjust dat file");
	string line;
	timepair tp;
	string date1,time1,date2,time2;
	while(!fdat.eof()){
		//#ң�����ʱ��                                     ң��֡ʱ��
		//2018-10-01 00:30:22.000                  2018-10-01 00:30:20.000
		//2018-10-01 00:30:23.000                  2018-10-01 00:30:21.000
		if( GetLine(&fdat,line) ) {
			stringstream ss(line);
			ss >> date1 >> time1 >> date2 >> time2;
			tp.ts = string2epoch(date1 + " " + time1);
			tp.re = string2epoch(date2 + " " + time2);
			tl.push_back(tp);
		}
	}
}

double rou(CSatellite &sat,CFacility &fac)
{
	vec3 sp,sv;
	sat.GetECF(sp, sv);
	vec3 fp = fac.ECFPosition;
	return norm(sp-fp,2);
}

//! Уʱ���ļ���
double adjusttime()
{
	CSatellite sat;
	sat.Initialize(op.epoch - 3600.0*8.0, op.kp);
	sat.Mass0 = op.Mass;
	sat.AirDragArea = op.AirDragArea;
	sat.Propagate2Epoch(tl[0].ts);
	int n = tl.size();
	double dt,lastdt;
	double step,sfr;
	// ����ÿһ��ʱ���Ĵ���ʱ��,����ʱ��
	for(unsigned int i=0;i<n;i++)
	{
		dt = lastdt = 0;
		sat.Propagate2Epoch(tl[i].ts + delaysat);
		do{
			lastdt = dt;
			sfr = rou(sat,fac)/LightVel;
			dt = tl[i].re - (tl[i].ts + delaysat + delaygrd + sfr);
			step = (tl[i].ts + dt + delaysat) - sat.CurrentEpoch();
			if(step>0)
				sat.Propagate(1, step);
			else
				sat.PropagateBackward(-1, step);
		}while(fabs(lastdt-dt)>1e-6);
		tl[i].dt = dt;
	}

	// ���ֵ����ֵ��б��
	double sum = 0;
	for(unsigned int i=0;i<n;i++)
	{
		sum += tl[i].dt;
	}
	double meandt = sum/n; // ����Уʱ
	double avgdt = 0;
	if(n>1)
		avgdt = (tl[n-1].dt - tl[0].dt)/(tl[n-1].ts - tl[0].ts)*3600;

	fstream ff;
	ff.open("adjusttime.txt",ios::out);
	ff << "#����Уʱʱ��ֵ\n";
	ff << "FocusAdjustTime = " << meandt << "\n";
	ff << "#����Уʱ����\n";
	ff << "UniformAdjustTimePeriod = " << 3600 << "\n";
	ff << "#����УʱУ����\n";
	ff << "UniformAdjustTime = " << avgdt << endl;
	ff.close();

	return 1;
}

void maketestdata(double timebias,double timerate)
{
	CSatellite sat;
	sat.Initialize(op.epoch - 3600.0*8.0, op.kp);
	sat.Mass0 = op.Mass;
	sat.AirDragArea = op.AirDragArea;
	double step = 60;
	// ����ÿһ��ʱ���Ĵ���ʱ��,����ʱ��
	double tanstime = 0;
	fstream tlf;
	tlf.open("timelist.txt",ios::out);
	int n = 200;

	CDateTime TmTime; // ң����ʱ��
	for(unsigned int i=0; i<n; i++) {
		TmTime = sat.CurrentEpoch();
		tlf << TmTime + (timebias+timerate*i*step) << TAB;
		sat.Propagate(delaysat, delaysat);
		tanstime = rou(sat,fac)/LightVel;
		tlf << TmTime + delaysat + tanstime + delaygrd << TAB << rou(sat,fac) << endl;
		sat.Propagate(step-delaysat,step-delaysat);
	}
	tlf.close();
}


int main(int argc, char* argv[])
{
	fstream ff;
	ff.open("adjusttime.txt",ios::out);
	ff.close();

	//if(argc<4)
	//{
	//	printf("����δָ�������ļ�!!\n\n");
	//	printf("��ʹ������ָ����ã�\n");
	//	printf("  adjusttime orbit.txt fac.txt timelist.txt\n");
	//	printf("  ��һ���ļ�Ϊ��������ļ�\n");
	//	printf("  �ڶ����ļ�Ϊ����վ������ʱ�Ӳ����ļ�\n");
	//	printf("  �������ļ�Ϊʱ���ļ�\n");
	//	printf(" ����ļ�Ϊadjusttime.txt\n");
	//	return 0;
	//}

	try{
		op = LoadOrbitFile(string(argv[1]));
		LoadFacFile(string(argv[2]));
		LoadTimeFile(string(argv[3]));

		//op = LoadOrbitFile("orbit.txt");
		//LoadFacFile("fac.txt");

		//double timebias = -0.02; // �����Ӳ�
		//double timerate = -0.001/3600; //������Ư
		//maketestdata(timebias,timerate);
		//LoadTimeFile("timelist.txt");

		adjusttime();
	}
	catch(BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
 	catch(exception* e)
 	{
 		cerr << ((BaseException*)e)->what() << endl;
 	}

	return 1;
}

