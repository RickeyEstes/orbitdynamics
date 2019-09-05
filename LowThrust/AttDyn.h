#pragma once

#include <cmath>
#include "..\orbitdyn\armadillo.h"
#include "..\orbitdyn\Quaternion.h"
#include "..\orbitdyn\Euler.h"

class AttDyn
{
public:
	AttDyn();
	~AttDyn();

	vec7 rigid(double t, vec7 x);
	double rigidstep(double h);

	CQuaternion q;
	vec3 w;
	mat33 Is;
	vec3 Torque;
	
	vec3 bc;	   //��������λ��,m
	
	int NThrustor; //������������
	vec3* ThrPos;  //����������װλ��,m
	vec3* ThrDir;  //����������������
	void SetThrustNum(int n) {
		NThrustor = n;
		ThrPos = new vec3[n];
		ThrDir = new vec3[n];
	}
	vec3 TThrust; // �������ϳ�����
	void Thrust(int * Open, double * FThr);
};

