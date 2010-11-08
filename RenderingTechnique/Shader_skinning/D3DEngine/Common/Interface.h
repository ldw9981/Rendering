#pragma once

interface IUnknownObject
{
	virtual ~IUnknownObject(){};
};

class cIRenderable
{
public:
	list<cIRenderable*>::iterator m_ItRenderable;
	virtual void Render()=0;
};

class cIControlable
{
public:
	list<cIControlable*>::iterator m_ItControlable;
	virtual void Control()=0;
};

class cIProgressable
{
public:
	list<cIProgressable*>::iterator m_ItProgressable;
	virtual void Update(DWORD elapseTime)=0;
};


