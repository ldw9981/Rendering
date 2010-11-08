#include "StdAfx.h"
#include "EnvironmentVariable.h"

EnvironmentVariable::EnvironmentVariable(void)
{

}

EnvironmentVariable::~EnvironmentVariable(void)
{
}


void EnvironmentVariable::SetString( string name,string strIn )
{
	m_mapEnvironmentVarString[name]=strIn;
}

string& EnvironmentVariable::GetString( string name)
{
	string& strOut = m_mapEnvironmentVarString[name];
	return strOut;
}

EnvironmentVariable& EnvironmentVariable::GetInstance()
{
	static EnvironmentVariable instance;
	return instance;
}