#pragma once
namespace Sophia
{


class cIRestore;
class cStaticRestoreList
{
public:
	cStaticRestoreList(){};
	~cStaticRestoreList(){};
protected:
	static std::list<cIRestore*>			m_listRestore;
};

class cIRestore:
	public cStaticRestoreList
{
public:
	cIRestore(void);
	virtual ~cIRestore(void);
	

	std::list<cIRestore*>::iterator		m_ItRestore;	//�ڱ��ڽ��� ����Ű�� ���ͷ������� ����
public:
	virtual void Restore()=0;
};
}