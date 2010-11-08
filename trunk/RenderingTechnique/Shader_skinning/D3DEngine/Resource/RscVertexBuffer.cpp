#include "stdafx.h"
#include "RscVertexBuffer.h"



cRscVertexBuffer::cRscVertexBuffer(void)
{
	SetType(VERTEXBUFFER);
	m_pD3DVertexBuffer=NULL;
}

cRscVertexBuffer::~cRscVertexBuffer(void)
{
	
}


/*
	미리 설정한 버퍼 사이즈로 할당만 한다.
*/
BOOL cRscVertexBuffer::Create()
{
	assert(m_RefCounter>=0);
	if (AddRef()!=1)
		return TRUE;

	HRESULT hResult;
	
	// ! VertexBuffer
	// 1) FVF를 설정해서 필요한크기만큼 VertexBuffer를 만든다.	^
	hResult=m_pD3DDevice9->CreateVertexBuffer( 
			m_BufferSize,
			0,
			0,
			D3DPOOL_DEFAULT,
			&m_pD3DVertexBuffer,
			NULL);

	assert(hResult==S_OK);
	MakeUniqeKey();
	return TRUE;
}



void cRscVertexBuffer::Free()
{
	SAFE_RELEASE(m_pD3DVertexBuffer);
	GetResourceMng()->EraseResource(GetUniqeKey());
	delete this;
}

void cRscVertexBuffer::Restore()
{

}



LPVOID cRscVertexBuffer::Lock()
{
	LPVOID pVertices;
	m_pD3DVertexBuffer->Lock( 0, m_BufferSize, (void**)&pVertices, 0 ) ;
	return pVertices;
}

VOID cRscVertexBuffer::Unlock()
{
	m_pD3DVertexBuffer->Unlock();
}