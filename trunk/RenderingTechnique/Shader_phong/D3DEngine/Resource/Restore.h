#pragma once

class cIRestore;
class cStaticRestoreList
{
public:
	cStaticRestoreList(){};
	~cStaticRestoreList(){};
protected:
	static list<cIRestore*>			m_listRestore;
};

class cIRestore:
	public cStaticRestoreList
{
public:
	cIRestore(void);
	virtual ~cIRestore(void);
	

	list<cIRestore*>::iterator		m_IterMyself;	//자기자신을 가리키는 이터레이터의 보관
public:
	virtual void Restore()=0;
};
