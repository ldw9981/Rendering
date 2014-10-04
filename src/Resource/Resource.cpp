#include "stdafx.h"
#include "Resource.h"
#include "Foundation/Trace.h"


namespace Sophia
{


UINT cReferenceCounter::Release()
{ 
	assert(m_RefCounter>0);
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

}

