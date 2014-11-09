#include "stdafx.h"
#include "RscIndexBuffer.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"
#include "Graphics/Graphics.h"

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
	assert(m_RefCounter>=0);
	
	if (m_RefCounter > 0 )
		return TRUE;

	HRESULT hResult;		
	// ! IndexBuffer
	// 1) �ʿ��� Index���� ��ŭ�� ũ��� IndexBuffer�� �����.
	hResult=Graphics::m_pDevice->CreateIndexBuffer( 
		m_BufferSize,
		m_usage,
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
	cResourceMng::m_pInstance->EraseRscIndexBuffer(m_strUniqeKey);
	delete this;
}

void* cRscIndexBuffer::Lock(UINT OffsetToLock,UINT SizeToLock,DWORD Flags)
{
	void *pIndices;
	m_pD3DIndexBuffer->Lock( OffsetToLock, SizeToLock, (void**)&pIndices,Flags);		
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