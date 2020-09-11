// LTOrbitControl.cpp: �������̨Ӧ�ó������ڵ㡣
//
#include <iostream>
#include <string>
using namespace std;

#include <orbitdyn.h>
using namespace Constant;

CSatellite sat;

struct OConInit
{
	string epoch;
	double a;	//�볤��(km)
	double e;	//ƫ����
	double i;	//���(deg)
	double Omega;	//������ྶ(deg)
	double w;	//���ص����(deg)
	double M;	//ƽ�����(deg)
	double Mass;	//���ǳ�ʼ����(kg)
	double Area;  // ��������ӭ�����(m^2)
	double F;     // ���ƽ�����(N)
	double Isp;   // ���ƽ��ȳ�(s)
};

void OrbitControl_Init(struct OConInit ocon)
{
	//char c[100] = "2019-01-01 00:00:00";
	int y, m, d, h, min;
	double sec;
	sscanf_s(ocon.epoch.c_str(), "%d-%d-%d %d:%d:%lf", &y, &m, &d, &h, &min, &sec);
	//CDateTime ep = StrToDateTime(c);
	//CDateTime ep = StrToDateTime(ocon.epoch.c_str());
	CDateTime ep(y, m, d, h, min, sec);
	Kepler kp;
	kp.a = ocon.a;
	kp.e = ocon.e;
	kp.i = ocon.i*RAD;
	kp.o = ocon.Omega*RAD;
	kp.w = ocon.w*RAD;
	kp.M = ocon.M*RAD;
	sat.Initialize(ep, kp);
	sat.Mass0 = ocon.Mass;
	sat.AirDragArea = ocon.Area;
	sat.SetEngine(ocon.Isp, ocon.F);
}

struct OConIn {
	//control type:
	//1: a
	//2: e
	//3: only e
	//4: i
	//5: a and i
	//6: e and i
	//7: a e i
	//8: Omega
	int OConType;
	// control target value
	double Target_a;
	double Target_e;
	double Target_i;
	double Target_Omega;
	// a e i costate var,  only available in type 7
	double lmda;
	double lmde;
	double lmdi;
};

struct OConOut {
	string t;		//��ʼ���������Ӧ�ı���ʱ�䣨�磺2018 - 01 - 01 00:00 : 00)
	double a;		//�볤��(km)
	double e;		//ƫ����
	double i;		//���(deg)
	double Omega;	//������ྶ(deg)
	double w;		//���ص����(deg)
	double M;		//ƽ�����(deg)
	double Mass;	//���ǳ�ʼ����(kg)
	double x;		//�����ڹ���ϵ�е�λ��X����(km)
	double y;		//�����ڹ���ϵ�е�λ��Y����(km)
	double z;		//�����ڹ���ϵ�е�λ��Z����(km)
	double vx;		//�����ڹ���ϵ�е��ٶ�X����(km/s)
	double vy;		//�����ڹ���ϵ�е��ٶ�Y����(km/s)
	double vz;		//�����ڹ���ϵ�е��ٶ�Z����(km/s)
	double Lon;		//���ľ���(deg)
	double Lat;		//����γ��(deg)
	double Alt;		//�߶�(km)
	int  WThr;	    //������������״̬,0�أ�1��
	double Fo[3];   //���ƽ����������ڹ��ϵ���������(N)
};

void OrbitControl_Step(double steptime,struct OConIn & oci, struct OConOut& oco)
{
	//OConType
	//1.�볤�����ſ��ƣ�
	//2.ƫ�������ſ��ƣ�
	//3.����ƫ���ʿ���
	//4.��ǿ���
	//5.�볤������Ǽ�Ȩ���ſ��ƣ�
	//6.ƫ��������Ǽ�Ȩ���ſ��ƣ�
	//7.�볤�ᡢƫ������������ſ��ƣ�
	//8.������ྭ���ſ��ơ�
	mat33 coi = GetCoi(sat.GetOrbitElements());

	//����·����, ��U������T����ļн�
	double beta = atan2(sat.e*sin(sat.f), 1.0 + sat.e * cos(sat.f)); 
	mat33 cui = RotationY(beta)*coi;
	
	sat.ThrustIsOn = true;
	sat.qbi = coi;  // ��̬��������ϵ�غ�

	double e = sat.e;
	double f = sat.f;
	double E = sat.E;
	double sf = sin(f);
	double cf = cos(f);
	double cE = cos(E);
	double sE = sin(E);

	double PHI = 40 * RAD;
	double betastar;
	
	// a e i optimal control
	double a = sat.a;
	double v = sat.v;
	double r = sat.r;

	double ra = r / a;
	double ra2 = ra * ra;
	double sa = -oci.lmde * ra * sf;
	double ca = 2 * (oci.lmda*a*a*v*v / GE + oci.lmde * (e + cf));
	double alphanorm = sqrt(sa * sa + ca * ca);
	double sinalphastar = sa / alphanorm;
	double cosalphastar = -ca / alphanorm;
	double h = sqrt(a*(1 - e * e));
	double di = oci.lmdi * r*v / h * cos(sat.w + f);
	double da = 2.0 * oci.lmda * a * a * v * v / GE * cosalphastar;
	double de = oci.lmde * ( 2 * (e + cf) * cosalphastar + ra * sf * sinalphastar );
	double d = sqrt(di * di + da * da + de * de);
	double sinbetastar = - di / d;
	double cosbetastar = (-da - de) / d;
	betastar = atan2(sinbetastar, cosbetastar)*DEG;
	double alphastar = atan2(sinalphastar, cosalphastar)*DEG;
	int incr = 1; // Increase:1  decrease:-1
	switch (oci.OConType)
	{
	case 1: // �볤�����ſ���
		incr = sat.a < oci.Target_a ? 1 : -1;
		sat.ThrustDirection(0) = incr * cos(beta);
		sat.ThrustDirection(1) = 0;
		sat.ThrustDirection(2) = -incr * sin(beta);
		break;
	case 2: // ƫ�������ſ���
		incr = sat.e < oci.Target_e ? 1 : -1;
		betastar = atan2(sf, cf + cE);
		sat.ThrustDirection(0) = incr * cos(betastar);
		sat.ThrustDirection(1) = 0;
		sat.ThrustDirection(2) = incr * sin(betastar);
		break;
	case 3: // ����ƫ���ʿ���
		incr = sat.e < oci.Target_e ? 1 : -1;
		sat.ThrustDirection(0) = 0;
		sat.ThrustDirection(1) = 0;
		sat.ThrustDirection(2) = incr * 1;
		sat.qbi = mat33(RotationY(sat.f + PI / 2)*coi);
		break;
	case 4: // ��ǿ���
		sat.ThrustDirection(0) = 0;
		sat.ThrustDirection(2) = 0;
		incr = sat.i < oci.Target_i ? 1 : -1;
		if (sat.u > PI / 2.0 && sat.u < 1.5*PI)
		{
			sat.ThrustDirection(1) = incr * 1;
		}
		else
		{
			sat.ThrustDirection(1) = -incr * 1;
		}
		break;
	case 5:  //�볤������Ǽ�Ȩ���ſ���
		betastar = atan(-2.0*(1 + e * cf)*sqrt(1 + 2 * e*cf + e * e) / (1 - e * e) / cos(sat.u));
		if (oci.Target_i > sat.i)
			betastar = -betastar;
		if (oci.Target_a < sat.a)
			beta = beta + PI;
		sat.ThrustDirection(0) =  cos(betastar)*cos(beta);
		sat.ThrustDirection(1) = -sin(betastar);
		sat.ThrustDirection(2) = -sin(betastar)*sin(beta);
		break;
	case 6:  //ƫ��������Ǽ�Ȩ���ſ���
		betastar = atan(-(1 - e * e)*cos(sat.u) / (1 + cf * cE)*(1 + e * cf));
		if (oci.Target_i > sat.i)
			betastar = -betastar;
		sat.ThrustDirection(0) = cos(betastar)*cos(beta);
		sat.ThrustDirection(1) = -sin(betastar);
		sat.ThrustDirection(2) = -sin(betastar)*sin(beta);
		break;
	case 7:  //�볤�ᡢƫ������������ſ���
		sat.ThrustDirection(0) =  cosalphastar * cosbetastar;
		sat.ThrustDirection(1) =                -sinbetastar; 
		sat.ThrustDirection(2) =  sinalphastar * cosbetastar;
		break;
	case 8:  //������ྭ���ſ���
		sat.ThrustDirection(0) = 0;
		sat.ThrustDirection(2) = 0;
		incr = sat.Omega < oci.Target_Omega ? 1 : -1;
		if (sat.u > PI)
		{
			sat.ThrustDirection(1) = 1*incr;
		}
		else
		{
			sat.ThrustDirection(1) = -1*incr;
		}
		break;
	default:
		return;
	}
	vec3 Fo = sat.ThrustDirection;
	sat.Propagate(steptime, steptime);

	CDateTime t = sat.CurrentEpoch();
	char tc[100];
	t.ToChar(tc);
	oco.t = tc;
	oco.a = sat.a;
	oco.e = sat.e;
	oco.i = sat.i*DEG;
	oco.Omega = sat.Omega*DEG;
	oco.w = sat.w*DEG;
	oco.M = sat.M*DEG;
	oco.Mass = sat.Mass();
	oco.x = sat.Pos()(0);
	oco.y = sat.Pos()(1);
	oco.z = sat.Pos()(2);
	oco.vx = sat.Vel()(0);
	oco.vy = sat.Vel()(1);
	oco.vz = sat.Vel()(2);
	CSpherical lla = sat.GetLLA();
	oco.Lon = lla.Longitude;
	oco.Lat = lla.Latitude;
	oco.Alt = lla.Altitude;
	oco.Fo[0] = Fo(0);
	oco.Fo[1] = Fo(1);
	oco.Fo[2] = Fo(2);
	oco.WThr = 1;
}

int main()
{
	// ��ʼ������
	struct OConInit ocon;
	// �����������
	struct OConIn oci;
	struct OConOut oco;

	//// �����ֵ
	//ocon.epoch = "2019-01-01 00:00:00";
	//ocon.a     = 7000;
	//ocon.e     = 0.001;
	//ocon.i     = 98*RAD;
	//ocon.Omega = 0;
	//ocon.w     = 0;
	//ocon.M     = 0;
	//ocon.Mass  = 60;
	//ocon.Area  = 0.16;
	//// ������С���ȳ�
	//ocon.F = 0.1;
	//ocon.Isp = 1000;

	// ���ļ��������ݣ��Ḳ������ĸ�ֵ
	BEGIN_READ("LTOCon.txt")
	READSTR(ocon.epoch)
	READVAR(ocon.a, "%lf")
	READVAR(ocon.e, "%lf")
	READVAR(ocon.i, "%lf")
	READVAR(ocon.Omega, "%lf")
	READVAR(ocon.w, "%lf")
	READVAR(ocon.M, "%lf")
	READVAR(ocon.Mass, "%lf")
	READVAR(ocon.Area, "%lf")
	READVAR(ocon.F, "%lf")
	READVAR(ocon.Isp, "%lf")
	END_READ

	// ��ʼ��
	OrbitControl_Init(ocon);

	// ���Ʋ���
	double step = 180;
	// �����ܲ���
	int len = 60 * 24 * 1; // 10��

	// ���STK�����ļ���ͷ��
	fstream fstk("OrbitControl.e", ios::out);
	fstk << "stk.v.4.3" << endl;
	fstk << "BEGIN Ephemeris" << endl;
	fstk << "NumberOfEphemerisPoints " << len << endl;
	fstk << "ScenarioEpoch           " << ocon.epoch << endl; //1 Jun 2002 12:00 : 00.000000000
	fstk << "InterpolationMethod     Lagrange" << endl;
	fstk << "InterpolationOrder      5" << endl;
	fstk << "CentralBody             Earth" << endl;
	fstk << "CoordinateSystem        J2000" << endl << endl;
	fstk << "EphemerisTimePosVel" << endl;
	fstk.precision(12);


	// ���������ļ�
	fstream fo("OrbitCon.txt", ios::out);
	fo.precision(12);

	for (int KK = 0;KK < len;KK++)
	{
		//OConType
		//1.�볤�����ſ��ƣ�
		//2.ƫ�������ſ��ƣ�
		//3.����ƫ���ʿ���
		//4.��ǿ���
		//5.�볤������Ǽ�Ȩ���ſ��ƣ�
		//6.ƫ��������Ǽ�Ȩ���ſ��ƣ�
		//7.�볤�ᡢƫ������������ſ��ƣ�
		//8.������ྭ���ſ��ơ�
		oci.OConType = 5;
		oci.Target_a = 42164.2;
		oci.Target_e = 0;
		oci.Target_i = 40*RAD;
		oci.Target_Omega = 190*RAD;
		// costate for a,e,e optim
		oci.lmda = 1;
		oci.lmde = 1;
		oci.lmdi = 1;
		// ����һ��
		OrbitControl_Step(step, oci, oco);

		// ���������ļ�
		fo << KK * step << "\t" << oco.a << "\t" << oco.e << "\t" << oco.i << "\t" << oco.Omega
			<< "\t" << oco.w << "\t" << oco.M << "\t"
			<< oco.x << "\t" << oco.y << "\t" << oco.z << "\t"
			<< oco.vx << "\t" << oco.vy << "\t" << oco.vz << "\t"
			<< oco.Lon << "\t" << oco.Lat << "\t" << oco.Alt << "\t"
			<< oco.Mass << "\t" << oco.Fo[0] << "\t" << oco.Fo[1] << "\t" << oco.Fo[2] <<"\n";

		// ���STK�����ļ�
		fstk << KK * step << "\t" << oco.x * 1000 << "\t" << oco.y * 1000 << "\t" << oco.z * 1000 << "\t"
			<< oco.vx * 1000 << "\t" << oco.vy * 1000 << "\t" << oco.vz * 1000 << "\n";

		// ����̨��ʾ
		printf("a = %lf e = %lf i = %lf Omega = %lf mass = %lf\n", oco.a,oco.e,oco.i,oco.Omega,oco.Mass);
	}

	fo.close();

	fstk << "END Ephemeris" << endl;
	fstk.close();

	return 0;
}
