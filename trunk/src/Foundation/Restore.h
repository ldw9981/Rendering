#pragma once
#include <list>

using namespace std;

class IRestore;
class cStaticRestoreList
{
public:
	cStaticRestoreList(){};
	~cStaticRestoreList(){};
protected:
	static list<IRestore*>			m_listRestore;
};

class IRestore:
	public cStaticRestoreList
{
public:
	IRestore(void);
	virtual ~IRestore(void);
	

	list<IRestore*>::iterator		m_ItRestore;	//자기자신을 가리키는 이터레이터의 보관
public:
	virtual void Restore()=0;
};
