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
	

	list<cIRestore*>::iterator		m_IterMyself;	//�ڱ��ڽ��� ����Ű�� ���ͷ������� ����
public:
	virtual void Restore()=0;
};
