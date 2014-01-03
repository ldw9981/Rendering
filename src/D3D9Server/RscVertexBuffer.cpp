#include "stdafx.h"
#include "RscVertexBuffer.h"
#include "Foundation/Define.h"


cRscVertexBuffer::cRscVertexBuffer(void)
{
	m_pD3DVertexBuffer=NULL;
	m_Type=D3DPOOL_DEFAULT;
	m_nCount=0;
}

cRscVertexBuffer::~cRscVertexBuffer(void)
{
	
}


/*
	�̸� ������ ���� ������� �Ҵ縸 �Ѵ�.
*/
BOOL cRscVertexBuffer::Create()
{
	assert(m_RefCounter>=0);
	if (AddRef()!=1)
		return TRUE;

	HRESULT hResult;
	
	// ! VertexBuffer
	// 1) FVF�� �����ؼ� �ʿ���ũ�⸸ŭ VertexBuffer�� �����.	^
	hResult=m_pD3DDevice->CreateVertexBuffer( 
			m_BufferSize,
			0,
			0,
			m_Type,
			&m_pD3DVertexBuffer,
			NULL);

	assert(hResult==S_OK);	
	return TRUE;
}



void cRscVertexBuffer::Free()
{
	SAFE_RELEASE(m_pD3DVertexBuffer);
	m_pResourceMng->EraseResource(GetUniqueKey());
	delete this;
}

void cRscVertexBuffer::Restore()
{

}



void* cRscVertexBuffer::Lock()
{
	void *pVertices=NULL;
	m_pD3DVertexBuffer->Lock( 0, m_BufferSize, (void**)&pVertices, 0 ) ;
	return pVertices;
}

void cRscVertexBuffer::Unlock()
{
	m_pD3DVertexBuffer->Unlock();
}



void cRscVertexBuffer::SetStreamSource( UINT stride )
{
	m_pD3DDevice->SetStreamSource( 0, m_pD3DVertexBuffer, 0,  stride );	
}

