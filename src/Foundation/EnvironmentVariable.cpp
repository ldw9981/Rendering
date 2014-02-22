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
}