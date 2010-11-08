#pragma once
// 멤버 데이터가 없는 순수 가상 인터페이스 c를 붙이지 않으며 I로 시작한다.
interface IUnknownObject
{
	virtual ~IUnknownObject(){};
};

class cIRenderable
{
public:
	list<cIRenderable*>::iterator m_IterMyself;
	virtual void Render()=0;
};

class cIControlable
{
public:
	list<cIControlable*>::iterator m_IterMyself;
	virtual void Control()=0;
};

class cIProgressable
{
public:
	list<cIProgressable*>::iterator m_IterMyself;
	virtual void Update()=0;
};


