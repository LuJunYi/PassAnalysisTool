#pragma once
#include <string>

class StringUtils
{
public:
	//��stringת����wstring  
	static std::wstring string2wstring(std::string str);
	
	//��wstringת����string  
	static std::string wstring2string(std::wstring wstr);
	
private:
	StringUtils();
	~StringUtils();
};

