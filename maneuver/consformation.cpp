#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

string outfilename, lanfilename, orbitfilename;
vector<double> lonlist;

int N = 0, P = 0, F = 0;
double Omega0, u0;

//! ����λ�ñ���
void consform()
{
	
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