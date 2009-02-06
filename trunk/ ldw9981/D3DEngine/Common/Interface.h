#pragma once

class IUnknownObject
{
public:
	virtual ~IUnknownObject(){};
};

class cIRenderer
{
public:
	virtual void Render()=0;
};

class cIRenderable
{
public:
	list<cIRenderable*>::iterator m_ItRenderable;
	virtual void ProcessRender()=0;
};

class cIControlable
{
public:
	list<cIControlable*>::iterator m_ItControlable;
	virtual void Control()=0;
};

class cIUpdatable
{
public:
	list<cIUpdatable*>::iterator m_ItProgressable;
	virtual void Update(DWORD elapseTime)=0;
};

class cGUIBase;
class cINotifiable
{
public:
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam)=0;
};

