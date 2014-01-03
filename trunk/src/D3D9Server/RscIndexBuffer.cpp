#include "stdafx.h"
#include "RscIndexBuffer.h"
#include "Foundation/Define.h"



cRscIndexBuffer::cRscIndexBuffer(void)
{	
	m_pD3DIndexBuffer=NULL;
	m_Type=D3DPOOL_DEFAULT;
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
	hResult=m_pD3DDevice->CreateIndexBuffer( 
		m_BufferSize,
		0,
		D3DFMT_INDEX16,
		m_Type,
		&m_pD3DIndexBuffer,
		NULL);
	assert(hResult==S_OK);
	
	return TRUE;
}


void cRscIndexBuffer::Restore()
{
	
}

void cRscIndexBuffer::Free()
{	
	SAFE_RELEASE(m_pD3DIndexBuffer);
	m_pResourceMng->EraseResource(m_strUniqeKey);
	delete this;
}

void* cRscIndexBuffer::Lock()
{
	void *pIndices;
	m_pD3DIndexBuffer->Lock( 0, m_BufferSize, (void**)&pIndices,0);		
	return pIndices;
}

VOID cRscIndexBuffer::Unlock()
{
	m_pD3DIndexBuffer->Unlock();	
}



void cRscIndexBuffer::SetIndices()
{
	m_pD3DDevice->SetIndices(m_pD3DIndexBuffer); 
}