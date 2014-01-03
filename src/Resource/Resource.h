#pragma once
#include "ResourceMng.h"
/*
	���ҽ��� �ɼ� �ִ°͵�?
	�ؽ���,�ε�������,���ؽ�����,�ִϸ��̼�����,��(�ؽ���+�ε�������+�ε�������+�ִϸ��̼�����)

	�ϳ��� ��������  �ʿ��� ���ҽ����� �ε����Ŀ� �� �ν��Ͻ��� ���ҽ����� �����Ͽ� �ν��Ͻ��������
	���� ������ �� ���ҽ����� �ݴ´�.
*/



class cReferenceCounter
{
public:
	cReferenceCounter(void) {m_RefCounter=0;}
	~cReferenceCounter(void) {}

	UINT	m_RefCounter;
public:
	UINT	AddRef();
	
	UINT	Release();	
	virtual void Free()=0;	
};

class cResource:
	public cReferenceCounter,
	public cStaticResourceMng
{
public:
	cResource(void);
	virtual ~cResource(void);
protected:
	std::string			m_strUniqeKey;			// ���ҽ� �Ŵ����� ���� ���� Ű	
public:
	
	const  std::string&	GetUniqueKey() const { return m_strUniqeKey; }
	void			SetUniqueKey(std::string& val) { m_strUniqeKey = val; }
	virtual BOOL	Create()=0;		// AddReference ȣ���� ���� ���ҽ��� �ش�Ǵ� D3D API�� ȣ��
};


