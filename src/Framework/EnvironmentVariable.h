#pragma once

class EnvironmentVariable
{
public:
//	EnvironmentVariable(void);
//	~EnvironmentVariable(void);
	/// get instance pointer
	static EnvironmentVariable& GetInstance();
	void SetString(string name,string strIn);
	string& GetString(string name);
private:
	EnvironmentVariable(void);
	~EnvironmentVariable(void);

protected:

	map<string,string >		m_mapEnvironmentVarString;
	map<string,bool>		m_mapEnvironmentVarBool;
	map<string,int>			m_mapEnvironmentVarInt;
	map<string,float>		m_mapEnvironmentVarFloat;	
};
