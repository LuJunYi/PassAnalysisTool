#include "pch.h"
#include "NRECToolPathHander.h"

#include <fstream>
#include <iostream>


#include "acis.hxx"
#include "kernapi.hxx"
#include "lists.hxx"
#include "position.hxx"
#include "vector.hxx"
#include "unitvec.hxx"
#include "cstrapi.hxx"
#include "ckoutcom.hxx"
#include "container_utils.hxx"

#include "SPAIAcisDocument.h"
#include "SPAIDocument.h"
#include "SPAIConverter.h"
#include "SPAIResult.h"

#include "StringUtils.h"

const std::string BPass = "BEGIN PASS";
const std::string EPass = "END PASS";
const std::string GOTO = "GOTO/";

bool NRECToolPathHander::Excuting(const std::wstring & toolPathName, double length, double radius)
{
	std::vector<std::vector<XYZIJK>> passVec;

	if (!ReadPassFile(toolPathName, passVec)) 
		return false;

	ENTITY_LIST ball_cente_list;//球心点集合
	ENTITY_LIST tool_vec_list; //刀轴矢量集合

	size_t pos1 = toolPathName.find_last_of('.');
	std::wstring tmp = toolPathName.substr(0, pos1);
	std::string pathEx = StringUtils::wstring2string(tmp);
	std::ofstream fOut(pathEx + "_ac.csv");

	API_BEGIN;
	int i = 1;
	for (auto pass : passVec)
	{
		fOut << " , , , , , , " << std::endl;//tcp.x,tcp.y,tcp.z,R,dis,A,C

		SPAposition_vector cur_pt_vec;
		int number = pass.size();

		double dPreZ = 0.0;
		double dPreR = -1.0;

		for (auto value : pass)
		{
			SPAposition pos(value.x, value.y, value.z);
			SPAvector vec(value.i, value.j, value.k);
			SPAunit_vector unitVec = normalise(vec);

			SPAposition cetPos = pos + unitVec * radius;
			SPAposition endPos = cetPos + unitVec * length;
			EDGE* ed = nullptr;
			check_outcome(api_mk_ed_line(cetPos, endPos, ed));
			if (ed != nullptr) {
				tool_vec_list.add(ed);
			}

			cur_pt_vec.push_back(cetPos);

			double R = sqrt(pow(cetPos.x(), 2) + pow(cetPos.y(), 2));

			double dis = 0.0;
			if (dPreR > 0.0) {
				dis = sqrt(pow(dPreZ - cetPos.z(), 2) + pow(R - dPreR, 2));
			}

			dPreR = R;
			dPreZ = cetPos.z();

			double avalue = acos(value.k) / M_PI * 180;
			double cvalue = atan2(value.i, value.j) / M_PI * 180;

			fOut << cetPos.x() << ", " << cetPos.y() << ", " << cetPos.z() << ", " << R << ", " << dis << ", " << avalue << ", " << cvalue << std::endl;
		}

		EDGE* cur_ed = nullptr;
		api_curve_spline(number, cur_pt_vec, nullptr, nullptr, cur_ed, 1, 1);
		if (cur_ed) {
			ball_cente_list.add(cur_ed);
		}
	}
	API_END;

	if (!result.ok())
	{
		std::string err(find_err_mess(result.error_number()));
		return false;
	}

	fOut.close();

	/*std::string tcp_path_sat = pathEx + "_tcp.sat";
	OutputSatFile(tcp_path_sat.c_str(), ball_cente_list);
	std::string tov_path_sat = pathEx + "_tov.sat";
	OutputSatFile(tov_path_sat.c_str(), tool_vec_list);*/

	std::string tcp_path_igs = pathEx + "_tcp.igs";
	if (transform(tcp_path_igs.c_str(), &ball_cente_list)) {
		transform(tcp_path_igs.c_str(), &ball_cente_list);
	}

	std::string tov_path_igs = pathEx + "_tov.igs";
	if (!transform(tov_path_igs.c_str(), &tool_vec_list)) {
		transform(tov_path_igs.c_str(), &tool_vec_list);
	}

	return true;
}

bool NRECToolPathHander::ReadPassFile(const std::wstring & toolPathName, std::vector<std::vector<XYZIJK>> &passVec)
{
	std::ifstream infile;
	infile.open(toolPathName, std::ios::in);
	if (!infile.is_open()) {
		return false;
	}

	std::vector<XYZIJK> pass;

	std::string::size_type pos;
	bool beginPass = false;
	std::string str;
	while (getline(infile, str))
	{
		pos = str.find(BPass);
		if (pos != std::string::npos)
		{
			beginPass = true;
			continue;
		}

		if (beginPass)
		{
			pos = str.find(GOTO);
			if (pos != std::string::npos)
			{
				XYZIJK val;
				sscanf(str.c_str(), "GOTO/%lf,%lf,%lf,%lf,%lf,%lf", &val.x, &val.y, &val.z, &val.i, &val.j, &val.k);
				pass.push_back(val);

				continue;
			}
		}

		pos = str.find(EPass);
		if (pos != std::string::npos)
		{
			beginPass = false;
			passVec.push_back(pass);
			pass.clear();
		}
	}

	return true;
}

void NRECToolPathHander::OutputSatFile(const char * output_path, ENTITY_LIST & entity_list)
{
	if (output_path == nullptr) {
		return;
	}
	if (entity_list.count() <= 0) {
		return;
	}
	API_NOP_BEGIN;

	// Set the units and product_id.
	FileInfo fileinfo;
	fileinfo.set_units(1.0);
	fileinfo.set_product_id("Example Application");
	result = api_set_file_info((FileIdent | FileUnits), fileinfo);
	check_outcome(result);

	//back-save SAT
	//api_save_version(int major_version, int minor_version)
	//result = api_save_version(20, 0);
	//check_outcome(result);

	// Also set the option to produce sequence numbers in the SAT file.
	result = api_set_int_option("sequence_save_files", 1);
	check_outcome(result);

	// Open a file for writing, save the list of entities, and close the file.
	FILE* save_file = fopen(output_path, "wb");
	result = api_save_entity_list(save_file, TRUE, entity_list);
	fclose(save_file);
	check_outcome(result);

	API_NOP_END;
}

bool NRECToolPathHander::transform(const char *filepath, ENTITY_LIST *list)
{
	try {
		SPAIAcisDocument src(list);
		SPAIDocument dst(filepath);
		SPAIConverter converter;
		SPAIResult res = converter.Convert(src, dst);
		return res.IsSuccess();
	}
	catch (...) {
		//qDebug() << "something wrong but ok";
		return false;
	}
	return true;
}