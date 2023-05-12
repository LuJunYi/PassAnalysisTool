#pragma once
#include <string>
#include <vector>

struct XYZIJK
{
public:
	double x = 0;
	double y = 0;
	double z = 0;

	double i = 0;
	double j = 0;
	double k = 0;
};

class ENTITY_LIST;

class NRECToolPathHander
{
public:
	NRECToolPathHander() = default;
	~NRECToolPathHander() = default;

	bool Excuting(const std::wstring& toolPathName, double length, double radius);

private:
	bool ReadPassFile(const std::wstring & toolPathName, std::vector<std::vector<XYZIJK>> &passVec);
	void OutputSatFile(const char* output_path, ENTITY_LIST& entity_list);
	bool transform(const char *filepath, ENTITY_LIST *list);
};

