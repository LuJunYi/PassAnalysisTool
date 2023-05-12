#pragma once
#include <string>

class StringUtils
{
public:
	//将string转换成wstring  
	static std::wstring string2wstring(std::string str);
	
	//将wstring转换成string  
	static std::string wstring2string(std::wstring wstr);
	
private:
	StringUtils();
	~StringUtils();
};

