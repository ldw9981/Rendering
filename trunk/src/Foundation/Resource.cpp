#include "Resource.h"
#include "../Foundation/Trace.h"




unsigned int ReferenceCounter::Release()
{ 
	m_RefCounter--; 
	if (m_RefCounter==0)	
	{
		Free();	
		// Release후 Free실행하여 인스턴스를 delete한경우
		// 더이상 인스턴스멤버는 엑세스할수없다 그러므로상수0리턴
		return 0;
	}
	return m_RefCounter;
}

unsigned int ReferenceCounter::AddRef()
{ 
	m_RefCounter++; 
	return m_RefCounter;
}


unsigned int cResource::m_nAccNumber=0;

cResource::cResource( void )
{
	m_nAccNumber++;	
	m_nUniqueNumber=m_nAccNumber;	
}	

cResource::~cResource( void )
{

}

/*
void cResource::ProcessMakeUniqueKey() 
{
	// 파일이름이 있으면 접두어_파일이름
	// 없으면 접두어_유니크번호
	WCHAR szBuffer[1024]={0,};
	switch(m_Type)	
	{
	case TEXTURE:
		swprintf_s(szBuffer,1024,L"TEXTURE_");
		break;
	case INDEXBUFFER:
		break;
		swprintf_s(szBuffer,1024,L"INDEXBUFFER_");
	case VERTEXBUFFER:
		swprintf_s(szBuffer,1024,L"VERTEXBUFFER_");
		break;
	}
	m_strUniqeKey = szBuffer;	

	if (!m_strFilePath.empty())
	{			
		m_strUniqeKey += m_strFilePath;		
		
	}
	else
	{
		WCHAR buffer[4];
		_itow_s(m_nUniqueNumber,buffer,4,10);
		m_strUniqeKey += buffer;
	}
		
	
}
*/

