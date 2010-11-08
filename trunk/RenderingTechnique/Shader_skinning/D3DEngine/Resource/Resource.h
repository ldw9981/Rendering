#pragma once
#include "ResourceMng.h"
/*
	리소스가 될수 있는것들?
	텍스쳐,인덱스버퍼,버텍스버퍼,애니메이션정보,씬(텍스쳐+인덱스버퍼+인덱스버퍼+애니메이션정보)

	하나의 씬을열때  필요한 리소스들을 로드한후에 각 인스턴스는 리소스들을 참조하여 인스턴스를만들고
	씬이 닫힐때 연 리소스들을 닫는다.
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
	static UINT		m_nAccNumber;			// 리소스 고유넘버를 위한 번호증가값 	
	
	enum RSCTYPE { TEXTURE,INDEXBUFFER,VERTEXBUFFER };
	
private:
	UINT			m_nUniqueNumber;				// 고유넘버
	RSCTYPE			m_Type;					// 리소스 타입

	wstring			m_strUniqeKey;			// 리소스 매니져에 사용될 맵의 키
	wstring			m_strFilePath;			// 리소스에 1:1대응될 파일 	


public:
	cResourceMng*	GetResourceMng() { return &m_ResourceMng; }
	UINT			GetType() const { return m_Type; }
	void			SetType(UINT val) { m_Type = (RSCTYPE)val; }

	void			SetFilePath(LPCWSTR szFilePath) { m_strFilePath=szFilePath; }
	const  wstring&	GetFilePath() const { return m_strFilePath; }

	void			MakeUniqeKey();  
	const  wstring&	GetUniqeKey() const { return m_strUniqeKey; }
	
	virtual BOOL	Create()=0;		// AddReference 호출후 실제 리소스에 해당되는 D3D API의 호출
};


