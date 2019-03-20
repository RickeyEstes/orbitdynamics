// ����̫������ָ�����
#include "mandef.h"
using namespace Constant;

string orbitfilename;
CDateTime epoch_start; // ��ʼʱ��
CDateTime epoch_stop; // ֹͣʱ��
double step; // ����

void LoadSAPFile(string filename)
{
	fstream file(filename, ios::in);
	if (!file.is_open())
		throw (string("Can't open file") + filename);
	std::string name, value;
	while (!file.eof()) {		
		if (ReadLine(&file, name, value)) {
			if (name == "step")
				sscanf(value.c_str(), "%lf", &step);
			else if (name == "epoch_stop")
				epoch_stop = string2epoch(value);
			else if (name == "epoch_start")
				epoch_start = string2epoch(value);

		}
	}
}

int main(int argc, char* argv[])
{
	if (argc<4)
	{
		printf("����δָ�������ļ�!!\n\n");
		printf("��ʹ������ָ����ã�\n");
		printf("  sap2 orbit.txt sap.txt sap2out.txt\n");
		printf("  ��һ���ļ�Ϊ��������ļ�\n");
		printf("  �ڶ����ļ�Ϊ����ָ����������ļ�\n");
		printf("  �������ļ�Ϊ����������ļ�,���зֱ�Ϊʱ�䡢alpha�ǡ�beta�ǡ�̫���ڹ������ϵ�ķ���\n");		
		return 0;
	}
	
	try {
		orbitfilename = string(argv[1]);

		LoadSAPFile(string(argv[2]));

		fstream fp(string(argv[3]), ios::out);

		CSatellite sat;
		InitSat(sat, orbitfilename);
		sat.Propagate2Epoch(epoch_start);
		vec3 si,vs,vo;
		mat coi;
		double alpha,beta;
		for (double t = 0;epoch_start + t <= epoch_stop;t += step) {
			si = Sun::Instance()->GetECI(epoch_start + t);
			vs = si - sat.Pos();
			vs = vs / norm(vs, 2);
			coi = GetCoi(sat.GetOrbitElements());
			vo = coi * vs;
			alpha = atan2(-vo(2), vo(0))*DEG;
			if (alpha < 0)
				alpha += 360;
			beta = asin(vo(1)) * DEG;
			fp << epoch_start + t << TAB << alpha << TAB << beta << TAB << vo.t();

			sat.Propagate(10, step);
		}

		
		fp.close();

	}
	catch (BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
	catch (exception* e)
	{
		cerr << ((BaseException*)e)->what() << endl;
	}
	
	return 0;
}
