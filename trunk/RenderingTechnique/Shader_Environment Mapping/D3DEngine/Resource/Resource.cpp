#include "stdafx.h"
#include "Resource.h"
#include "../Log/Trace.h"




UINT cReferenceCounter::Release()
{ 
	m_RefCounter--; 
	if (m_RefCounter==0)	Free();
	if (m_RefCounter<0) 
	{ 
		TRACE(_T("���ҽ��� Createȣ�⺸�� Destroyȣ�� Ƚ���� ����\n"));
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
	// �����̸��� ������ ���ξ�_�����̸�
	// ������ ���ξ�_����ũ��ȣ
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

