#include "stdafx.h"
#include "RscIndexBuffer.h"



cRscIndexBuffer::cRscIndexBuffer(void)
{
	SetType(INDEXBUFFER);
	m_pD3DIndexBuffer=NULL;
}

cRscIndexBuffer::~cRscIndexBuffer(void)
{
	
}

BOOL cRscIndexBuffer::Create()
{
	assert(m_RefCounter>=0);
	if (AddRef()!=1)
		return TRUE;

	HRESULT hResult;		
	// ! IndexBuffer
	// 1) 필요한 Index갯수 만큼의 크기로 IndexBuffer를 만든다.
	hResult=m_pD3DDevice9->CreateIndexBuffer( 
		m_BufferSize,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&m_pD3DIndexBuffer,
		NULL);
	assert(hResult==S_OK);
	MakeUniqeKey();
	return TRUE;
}


void cRscIndexBuffer::Restore()
{
	
}

void cRscIndexBuffer::Free()
{	
	SAFE_RELEASE(m_pD3DIndexBuffer);
	GetResourceMng()->EraseResource(GetUniqeKey());
	delete this;
}

LPVOID cRscIndexBuffer::Lock()
{
	LPVOID* pIndices;
	m_pD3DIndexBuffer->Lock( 0, m_BufferSize, (void**)&pIndices,0);		
	return pIndices;
}

VOID cRscIndexBuffer::Unlock()
{
	m_pD3DIndexBuffer->Unlock();	
}