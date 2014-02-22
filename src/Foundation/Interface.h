#pragma once
namespace Sophia
{

class IRenderable;
class IControlable;
class INotifiable;
class cGUIBase;
class IUpdatable;

class IUnknownObject
{
public:
	virtual ~IUnknownObject(){};
};

class IRenderer
{
public:
	virtual void Render(){};
	virtual void Render(unsigned char multiSubIndex){};
};

class IRenderable
{
public:
	std::list<IRenderable*>::iterator m_ItRenderable;
	virtual void ProcessRender()=0;
};

class IControlable
{
public:
	std::list<IControlable*>::iterator m_ItControlable;
	virtual void Control()=0;
};

class IUpdatable
{
public:
	std::list<IUpdatable*>::iterator m_ItProgressable;
	virtual void Update(DWORD elapseTime)=0;
};


class INotifiable
{
public:
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam)=0;
};


class ISerializable
{
public:
	virtual void SerializeIn(std::ifstream& stream)=0;
	virtual void SerializeOut(std::ofstream& stream)=0;

	void ReadString( std::ifstream& stream,std::string& str );
	void WriteString( std::ofstream& stream,std::string& str );

	void ReadMatrix(std::ifstream& stream,D3DXMATRIX& mat);
	void WriteMatrix(std::ofstream& stream,D3DXMATRIX& mat);

	void ReadFloat(std::ifstream& stream,float& var);
	void WriteFloat(std::ofstream& stream,float& var);
};

}