#include "stdafx.h"
#include "RscIndexBuffer.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"
#include "Graphics/Graphics.h"
#include "Foundation/Trace.h"

using namespace Sophia;

cRscIndexBuffer::cRscIndexBuffer(void)
{	
	m_pD3DIndexBuffer=NULL;
	m_Type=D3DPOOL_MANAGED;
	m_usage=D3DUSAGE_WRITEONLY;
}

cRscIndexBuffer::~cRscIndexBuffer(void)
{
	
}

BOOL cRscIndexBuffer::Create()
{
	HRESULT hr;

	assert(m_RefCounter>=0);	

	if (m_RefCounter > 0 )
		return TRUE;

	// ! IndexBuffer
	// 1) 필요한 Index갯수 만큼의 크기로 IndexBuffer를 만든다.
	V(Graphics::m_pDevice->CreateIndexBuffer( 
		m_BufferSize,
		m_usage,
		D3DFMT_INDEX16,
		m_Type,
		&m_pD3DIndexBuffer,
		NULL));

	if (FAILED(hr))
	{
		return FALSE;
	}	
	return TRUE;
}


void cRscIndexBuffer::Restore()
{
	
}

void cRscIndexBuffer::Free()
{	
	SAFE_RELEASE(m_pD3DIndexBuffer);
	cResourceMng::m_pInstance->EraseRscIndexBuffer(m_strUniqeKey);
	delete this;
}

void* cRscIndexBuffer::Lock(UINT SizeToLock,DWORD Flags)
{
	void *pIndices;
	m_pD3DIndexBuffer->Lock(0, SizeToLock, (void**)&pIndices,Flags);		
	return pIndices;
}

VOID cRscIndexBuffer::Unlock()
{
	m_pD3DIndexBuffer->Unlock();	
}



void cRscIndexBuffer::SetIndices()
{
	Graphics::m_pDevice->SetIndices(m_pD3DIndexBuffer); 
}