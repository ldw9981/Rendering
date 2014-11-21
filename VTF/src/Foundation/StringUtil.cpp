#include "StdAfx.h"
#include "StringUtil.h"

namespace Sophia
{

StringUtil::StringUtil(void)
{
}


StringUtil::~StringUtil(void)
{
}

void StringUtil::SplitPath(std::string& input,std::string* _Drive,std::string* _Dir,std::string* _Filename,std::string* _Ext )
{
	char drive[256]={0,};
	char dir[256]={0,};
	char filename[256]={0,};
	char ext[256]={0,};
	_splitpath_s(input.c_str(),drive,256,dir,256,filename,256,ext,256);
	if (_Drive!=NULL)
	{
		*_Drive += drive;
	}
	if (_Dir!=NULL)
	{
		*_Dir += dir;
	}
	if (_Filename!=NULL)
	{
		*_Filename += filename;
	}

	if (_Ext!=NULL)
	{
		*_Ext += ext;
	}
}
void StringUtil::LowerCase( std::string& s )
{
	

	int dif='a'-'A';
	for(size_t i=0;i<s.length();i++)
	{
		if((s[i]>='A')&&(s[i]<='Z'))
			s[i]+=dif;
	}
}

void StringUtil::UpperCase( std::string& s )
{
	int dif='a'-'A';
	for(size_t i=0;i<s.length();i++)
	{
		if((s[i]>='a')&&(s[i]<='z'))
			s[i]-=dif;
	}
}

}