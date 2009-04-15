#pragma once

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

class cGUIBase;
class INotifiable
{
public:
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam)=0;
};

