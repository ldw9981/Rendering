#include "stdafx.h"
#include "Resource.h"
#include "../Log/Trace.h"




UINT cReferenceCounter::Release()
{ 
	m_RefCounter--; 
	if (m_RefCounter==0)	Free();
	if (m_RefCounter<0) 
	{ 
		TRACE(_T("리소스의 Create호출보다 Destroy호출 횟수가 많음\n"));
		m_RefCounter=0;
	}
	return m_RefCounter;
}

UINT cReferenceCounter::AddReference()
{ 
	m_RefCounter++; 
	return m_RefCounter;
}


UINT cResource::m_nAccNumber=0;

cResource::cResource( void )
{
	m_nAccNumber++;	
	m_nUniqueNumber=m_nAccNumber;	
}	

cResource::~cResource( void )
{

}

void cResource::MakeUniqeKey() 
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

