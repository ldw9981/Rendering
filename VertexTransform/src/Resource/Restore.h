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
	

	std::list<cIRestore*>::iterator		m_ItRestore;	//자기자신을 가리키는 이터레이터의 보관
public:
	virtual void Restore()=0;
};
}