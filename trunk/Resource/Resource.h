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
	private cStaticResourceMng
{
public:
	cResource(void);
	virtual ~cResource(void);
public:
	static UINT		m_nAccNumber;			// ���ҽ� �����ѹ��� ���� ��ȣ������ 	
	
private:
	UINT			m_nUniqueNumber;				// �����ѹ�


	string			m_strUniqeKey;			// ���ҽ� �Ŵ����� ���� ���� Ű	
	string			m_strFilePath;			// ���ҽ��� 1:1������ ���� 	


public:
	cResourceMng*	GetResourceMng() { return &m_ResourceMng; }


	void			SetFilePath(const char* szFilePath) { m_strFilePath=szFilePath; }
	const  string&	GetFilePath() const { return m_strFilePath; }

	
	const  string&	GetUniqueKey() const { return m_strUniqeKey; }
	void			SetUniqueKey(string& val) { m_strUniqeKey = val; }
	UINT			GetUniqueNumber() const { return m_nUniqueNumber; }
	

	virtual void	ProcessMakeUniqueKey()=0;  
	virtual BOOL	Create()=0;		// AddReference ȣ���� ���� ���ҽ��� �ش�Ǵ� D3D API�� ȣ��
};


