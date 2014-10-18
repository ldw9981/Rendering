#pragma once
/*
	리소스가 될수 있는것들?
	텍스쳐,인덱스버퍼,버텍스버퍼,애니메이션정보,씬(텍스쳐+인덱스버퍼+인덱스버퍼+애니메이션정보)

	하나의 씬을열때  필요한 리소스들을 로드한후에 각 인스턴스는 리소스들을 참조하여 인스턴스를만들고
	씬이 닫힐때 연 리소스들을 닫는다.
*/
namespace Sophia
{



class cReferenceCounter
{
public:
	cReferenceCounter(void) {m_RefCounter=0;}
	~cReferenceCounter(void) {}

	UINT	m_RefCounter;
	
public:
	UINT	AddRef();
	UINT	GetRefCounter() const { return m_RefCounter; }
	UINT	Release();	
	virtual void Free()=0;	
};

//template <typename T> 
class cResource:
	public cReferenceCounter
{
public:
	cResource( void )
	{

	}

	~cResource( void )
	{

	}
protected:
	std::string			m_strUniqeKey;			// 리소스 매니져에 사용될 맵의 키	

	//T					m_key;
public:
	
	const  std::string&	GetUniqueKey() const { return m_strUniqeKey; }
	void			SetUniqueKey(std::string& val) { m_strUniqeKey = val; }
	virtual BOOL	Create()=0;		// AddReference 호출후 실제 리소스에 해당되는 D3D API의 호출

	/*
	void SetKey(T& key) {m_key = key; }
	const T GetKey() { return m_key; }
	*/
};


}