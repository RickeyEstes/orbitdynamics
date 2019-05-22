#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

string outfilename, lanfilename, orbitfilename;
//#���������
double Lon = 120;
//#Զ�ص������
int Nap = 5;
//#��صĵ����ȷ�Χ
double minLon = 20;
double maxLon = 150;
//#������������
double F = 490;
//#���������ȳ�
double Isp = 305;

//! GEO���ת��
void geotrans()
{
	

	fstream fout;
	fout.open(outfilename, ios::out);
	for (int k = 1; k <= Nap; k++)
	{
		fout << "��" << k << "�ε��\n";
		fout << "����ʱ�� = " << endl;
		fout << "�ػ�ʱ�� = " << endl;
		fout << "����ʱ��(s) = " << endl;
		fout << "�ٶ�����(m/s) = " << endl;
		fout << "�����ྭ = " << endl;
		fout << "������γ = " << endl;
		fout << "�ٶ����� = " << endl;
		fout << "ȼ������ = " << endl;
		fout << "����������� = " << endl;
		fout << "���������γ�� = " << endl;
		fout << "�������������� = " << endl;
		fout << "�ػ�������� = " << endl;
		fout << "�ػ������γ�� = " << endl;
		fout << "�������������� = " << endl;
		fout << "�������������� = " << endl;
		fout << "�ػ����������� = " << endl;
		fout << "\n";
	}	
	fout.close();
}

void loadcon(string lanfilename) {
	fstream flan(lanfilename, ios::in);
	double lon;
	string name, value;
	int N = 0, P = 0, F = 0;
	double Omega0, u0;
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
		//Isp = 305
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