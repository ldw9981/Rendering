#include "stdafx.h"
#include "Resource.h"
#include "Foundation/Trace.h"




UINT cReferenceCounter::Release()
{ 
	m_RefCounter--; 
	if (m_RefCounter==0)	
	{
		Free();	
		// Release�� Free�����Ͽ� �ν��Ͻ��� delete�Ѱ��
		// ���̻� �ν��Ͻ������ �������Ҽ����� �׷��Ƿλ��0����
		return 0;
	}
	return m_RefCounter;
}

UINT cReferenceCounter::AddRef()
{ 
	m_RefCounter++; 
	return m_RefCounter;
}

cResource::cResource( void )
{

}	

cResource::~cResource( void )
{

}



