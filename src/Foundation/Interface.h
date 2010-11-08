#pragma once

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
	virtual void Render()=0;
};

class IRenderable
{
public:
	list<IRenderable*>::iterator m_ItRenderable;
	virtual void ProcessRender()=0;
};

class IControlable
{
public:
	list<IControlable*>::iterator m_ItControlable;
	virtual void Control()=0;
};

class IUpdatable
{
public:
	list<IUpdatable*>::iterator m_ItProgressable;
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
	virtual void SerializeIn(fstream& in)=0;
	virtual void SerializeOut(fstream& out)=0;
};
