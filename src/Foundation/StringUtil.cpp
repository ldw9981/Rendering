#include "StdAfx.h"
#include "StringUtil.h"


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
