#pragma once
// ��� �����Ͱ� ���� ���� ���� �������̽� c�� ������ ������ I�� �����Ѵ�.
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


