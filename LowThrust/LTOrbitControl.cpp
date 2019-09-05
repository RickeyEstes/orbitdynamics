// LTOrbitControl.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;

#include <orbitdyn.h>
using namespace Constant;

CSatellite sat;

struct OConInit
{
	string epoch;
	double a;	//半长轴(km)
	double e;	//偏心率
	double i;	//倾角(deg)
	double Omega;	//升交点赤径(deg)
	double w;	//近地点幅角(deg)
	double M;	//平近点角(deg)
	double Mass;	//卫星初始质量(kg)
	double Area;  // 大气阻力迎风面积(m^2)
	double F;     // 电推进推力(N)
	double Isp;   // 电推进比冲(s)
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

struct OConOut {
	string t;		//初始轨道参数对应的北京时间（如：2018 - 01 - 01 00:00 : 00)
	double a;		//半长轴(km)
	double e;		//偏心率
	double i;		//倾角(deg)
	double Omega;	//升交点赤径(deg)
	double w;		//近地点幅角(deg)
	double M;		//平近点角(deg)
	double Mass;	//卫星初始质量(kg)
	double x;		//卫星在惯性系中的位置X分量(km)
	double y;		//卫星在惯性系中的位置Y分量(km)
	double z;		//卫星在惯性系中的位置Z分量(km)
	double vx;		//卫星在惯性系中的速度X分量(km/s)
	double vy;		//卫星在惯性系中的速度Y分量(km/s)
	double vz;		//卫星在惯性系中的速度Z分量(km/s)
	double Lon;		//地心经度(deg)
	double Lat;		//地心纬度(deg)
	double Alt;		//高度(km)
	double Fair[3]; //大气阻力在卫星本体系的三轴分量(N)
	int  WThr;	    //电推力器开关状态,0关，1开
	double Fo[3];   //电推进推力方向在轨道系的三轴分量(N)
};

void OrbitControl_Step(double steptime,int OConType, struct OConOut& oco)
{
	//OConType
	//1.半长轴最优控制；
	//2.偏心率最优控制；
	//3.半长轴与倾角加权最优控制；
	//4.偏心率与倾角加权最优控制；
	//5.半长轴、偏心率与倾角最优控制；
	//6.倾角与升交点赤经最优控制。
	mat33 coi = GetCoi(sat.GetOrbitElements());

	//飞行路径角, 即U方向与T方向的夹角
	double beta = atan2(sat.e*sin(sat.f), 1.0 + sat.e * cos(sat.f)); 
	mat33 cui = RotationY(beta)*coi;
	
	sat.ThrustIsOn = true;
	sat.qbi = coi;  // 姿态与轨道坐标系重合

	double f = sat.f;
	double E = sat.E;
	double sf = sin(f);
	double cf = cos(f);
	double cE = cos(E);
	double sE = sin(E);

	double PHI = 40 * RAD;

	switch (OConType)
	{
	case 1: // 半长轴最优控制
		sat.ThrustDirection(0) = cos(beta);
		sat.ThrustDirection(1) = 0;
		sat.ThrustDirection(2) = -sin(beta);
		break;
	case 2: // 偏心率最优控制
		sat.ThrustDirection(0) = -(cf + cE) / sqrt(sf*sf + (cf + cE)*(cf + cE));
		sat.ThrustDirection(1) = 0;
		sat.ThrustDirection(2) = -sf / sqrt(sf*sf + (cf + cE)*(cf + cE));
		break;
	case 3:  //半长轴与倾角加权最优控制
		
		break;
	case 4:  //偏心率与倾角加权最优控制

		break;
	case 5:  //半长轴、偏心率与倾角最优控制

		break;
	case 6:  //倾角与升交点赤经最优控制

		break;
	default:
		return;
	}
	vec3 Fo = sat.ThrustDirection*sat.Force();
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
	//vec3 Fair = PAirDrag()*sat.Mass();
	//oco.Fair[0] = Fair(0);
	//oco.Fair[1] = Fair(1);
	//oco.Fair[2] = Fair(2);
}

int main()
{
	// 初始化参数
	struct OConInit ocon;
	// 输入输出参数
	struct OConOut oco;

	//// 轨道初值
	//ocon.epoch = "2019-01-01 00:00:00";
	//ocon.a     = 7000;
	//ocon.e     = 0.001;
	//ocon.i     = 98*RAD;
	//ocon.Omega = 0;
	//ocon.w     = 0;
	//ocon.M     = 0;
	//ocon.Mass  = 60;
	//ocon.Area  = 0.16;
	//// 推力大小、比冲
	//ocon.F = 0.1;
	//ocon.Isp = 1000;

	// 从文件读入数据，会覆盖上面的赋值
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

	// 初始化
	OrbitControl_Init(ocon);

	// 外推步长
	double step = 60;
	// 外推总步数
	int len = 60*24 * 10; // 10天

	// 输出STK星历文件的头部
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


	// 保存数据文件
	fstream fo("OrbitCon.txt", ios::out);
	fo.precision(12);

	for (int KK = 0;KK < len;KK++)
	{
		// 外推一步
		OrbitControl_Step(step, 1, oco);

		// 保存数据文件
		fo << KK * step << "\t" << oco.a << "\t" << oco.e << "\t" << oco.i << "\t" << oco.Omega
			<< "\t" << oco.w << "\t" << oco.M << "\t"
			<< oco.x << "\t" << oco.y << "\t" << oco.z << "\t"
			<< oco.vx << "\t" << oco.vy << "\t" << oco.vz << "\t"
			<< oco.Lon << "\t" << oco.Lat << "\t" << oco.Alt << "\t"
			<< oco.Mass << "\n";

		// 输出STK星历文件
		fstk << KK * step << "\t" << oco.x * 1000 << "\t" << oco.y * 1000 << "\t" << oco.z * 1000 << "\t"
			<< oco.vx * 1000 << "\t" << oco.vy * 1000 << "\t" << oco.vz * 1000 << "\n";

		// 控制台提示
		printf("a = %lf mass = %lf\n", oco.a, oco.Mass);
	}

	fo.close();

	fstk << "END Ephemeris" << endl;
	fstk.close();

	return 0;
}
