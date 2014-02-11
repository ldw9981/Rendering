#pragma once

class EnvironmentVariable
{
public:
//	EnvironmentVariable(void);
//	~EnvironmentVariable(void);
	/// get instance pointer
	static EnvironmentVariable& GetInstance();
	void SetString(std::string name,std::string strIn);
	std::string& GetString(std::string name);
private:
	EnvironmentVariable(void);
	~EnvironmentVariable(void);

protected:

	std::map<std::string,std::string >		m_mapEnvironmentVarString;
	std::map<std::string,bool>		m_mapEnvironmentVarBool;
	std::map<std::string,int>			m_mapEnvironmentVarInt;
	std::map<std::string,float>		m_mapEnvironmentVarFloat;	
};
