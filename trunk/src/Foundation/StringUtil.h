#pragma once
class StringUtil
{
public:
	StringUtil(void);
	~StringUtil(void);

	static	void SplitPath(std::string& input,std::string* _Drive,std::string* _Dir,std::string* _Filename,std::string* _Ext );
};

