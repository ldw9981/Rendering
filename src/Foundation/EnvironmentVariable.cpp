#include "StdAfx.h"
#include "EnvironmentVariable.h"
namespace Sophia
{

EnvironmentVariable::EnvironmentVariable(void)
{

}

EnvironmentVariable::~EnvironmentVariable(void)
{
}


void EnvironmentVariable::SetString( std::string name,std::string strIn )
{
	m_mapEnvironmentVarString[name]=strIn;
}

std::string& EnvironmentVariable::GetString( std::string name)
{
	std::string& strOut = m_mapEnvironmentVarString[name];
	return strOut;
}

EnvironmentVariable& EnvironmentVariable::GetInstance()
{
	static EnvironmentVariable instance;
	return instance;
}

void EnvironmentVariable::SetPath()
{
	char szModuleFolder[1024] = { 0, };
	char* pszTemp;
	GetModuleFileName(GetModuleHandle(NULL), szModuleFolder, 1023);

	pszTemp = strrchr(szModuleFolder, '\\');
	if(pszTemp) {
		*pszTemp = 0;
	}	
	EnvironmentVariable::GetInstance().SetString("BinPath",std::string(szModuleFolder)+std::string("\\"));


	pszTemp = strrchr(szModuleFolder, '\\');
	if(pszTemp) {
		*pszTemp = 0;
	}	
	EnvironmentVariable::GetInstance().SetString("BasePath",std::string(szModuleFolder)+std::string("\\"));


	EnvironmentVariable::GetInstance().SetString("DataPath",std::string(szModuleFolder)+std::string("\\Data\\"));
}

}