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
	

	list<IRestore*>::iterator		m_ItRestore;	//�ڱ��ڽ��� ����Ű�� ���ͷ������� ����
public:
	virtual void Restore()=0;
};
