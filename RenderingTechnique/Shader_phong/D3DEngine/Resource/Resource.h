#pragma once

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
	UINT	AddReference();
	
	UINT	Release();	
	virtual void Free()=0;	
};

class cResource
	:public cReferenceCounter
{
public:
	cResource(void);
	virtual ~cResource(void);
public:
	static UINT		m_nAccNumber;			// ���ҽ� �����ѹ��� ���� ��ȣ������ 	
	
	enum RSCTYPE { TEXTURE,INDEXBUFFER,VERTEXBUFFER };
	
private:
	UINT			m_nUniqueNumber;				// �����ѹ�
	RSCTYPE			m_Type;					// ���ҽ� Ÿ��

	wstring			m_strUniqeKey;			// ���ҽ� �Ŵ����� ���� ���� Ű
	wstring			m_strFilePath;			// ���ҽ��� 1:1������ ���� 	


public:

	UINT			GetType() const { return m_Type; }
	void			SetType(UINT val) { m_Type = (RSCTYPE)val; }

	void			SetFilePath(LPCWSTR szFilePath) { m_strFilePath=szFilePath; }
	const  wstring&	GetFilePath() const { return m_strFilePath; }

	void			MakeUniqeKey();  
	const  wstring&	GetUniqeKey() const { return m_strUniqeKey; }
	
	virtual BOOL	Create()=0;		// ���� ���ҽ��� �ش�Ǵ� D3D API�� ȣ��
	virtual BOOL	Destroy()=0;	// D3DAPIȣ�⿡���� ����

};


