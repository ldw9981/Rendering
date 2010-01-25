#pragma once
#include "ResourceMng.h"
#include "ReferenceCounter.h"

/*
	���ҽ��� �ɼ� �ִ°͵�?
	�ؽ���,�ε�������,���ؽ�����,�ִϸ��̼�����,��(�ؽ���+�ε�������+�ε�������+�ִϸ��̼�����)

	�ϳ��� ��������  �ʿ��� ���ҽ����� �ε����Ŀ� �� �ν��Ͻ��� ���ҽ����� �����Ͽ� �ν��Ͻ��������
	���� ������ �� ���ҽ����� �ݴ´�.
*/




class cResource:
	public ReferenceCounter,
	public SharedResourceMng
{
public:
	cResource(void);
	virtual ~cResource(void);
public:
	static UINT		m_nAccNumber;			// ���ҽ� �����ѹ��� ���� ��ȣ������ 	
	
private:
	UINT			m_nUniqueNumber;				// �����ѹ�
	wstring			m_strUniqeKey;			// ���ҽ� �Ŵ����� ���� ���� Ű	
	wstring			m_strFilePath;			// ���ҽ��� 1:1������ ���� 	

public:

	void			SetFilePath(LPCWSTR szFilePath) { m_strFilePath=szFilePath; }
	const  wstring&	GetFilePath() const { return m_strFilePath; }

	
	const  wstring&	GetUniqueKey() const { return m_strUniqeKey; }
	void			SetUniqueKey(wstring& val) { m_strUniqeKey = val; }
	UINT			GetUniqueNumber() const { return m_nUniqueNumber; }
	

	virtual void	ProcessMakeUniqueKey()=0;  
	virtual BOOL	Create()=0;		// AddReference ȣ���� ���� ���ҽ��� �ش�Ǵ� D3D API�� ȣ��
};


