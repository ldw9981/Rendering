#pragma once

namespace Sophia
{

class StringUtil
{
public:
	StringUtil(void);
	~StringUtil(void);

	static	void SplitPath(std::string& input,std::string* _Drive,std::string* _Dir,std::string* _Filename,std::string* _Ext );
	static  void LowerCase(std::string& s);
	static	void UpperCase(std::string& s);
};

}

