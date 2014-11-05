#include "stdafx.h"
#include "RscVertexBuffer.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"
#include "Graphics/Graphics.h"
#include "Foundation/Trace.h"

using namespace Sophia;

cRscVertexBuffer::cRscVertexBuffer(void)
{
	m_pD3DVertexBuffer=NULL;
	m_pool=D3DPOOL_MANAGED;		// �⺻�޽��� �������ʴ� ���ؽ��̹Ƿ� ����� ����Ѵ�.
	m_vertexCount=0;
	m_usage = D3DUSAGE_WRITEONLY;
	m_OffSet =0;
	m_BufferSize =0;
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
	if (m_RefCounter > 0 )
		return TRUE;

	HRESULT hr;
	
	// ! VertexBuffer
	// 1) FVF�� �����ؼ� �ʿ���ũ�⸸ŭ VertexBuffer�� �����.	^
	V( Graphics::m_pDevice->CreateVertexBuffer(m_BufferSize,m_usage,0,	m_pool,&m_pD3DVertexBuffer,NULL));
	if (FAILED(hr))
		return FALSE;

	return TRUE;
}



void cRscVertexBuffer::Free()
{
	SAFE_RELEASE(m_pD3DVertexBuffer);
	if (!GetUniqueKey().empty())
	{
		cResourceMng::m_pInstance->EraseRscVertexBuffer(GetUniqueKey());
	}	
	delete this;
}

void cRscVertexBuffer::Restore()
{

}



void* cRscVertexBuffer::Lock(UINT SizeToLock,DWORD Flags)
{
	void *pVertices=NULL;
	HRESULT hr;
	V (m_pD3DVertexBuffer->Lock( m_OffSet, SizeToLock, (void**)&pVertices, Flags ));
	if(FAILED(hr))
		return NULL;

	return pVertices;
}

void cRscVertexBuffer::Unlock()
{
	HRESULT hr;
	V( m_pD3DVertexBuffer->Unlock());
}



void cRscVertexBuffer::SetStreamSource(UINT streamNumber,  UINT stride )
{
	Graphics::m_pDevice->SetStreamSource( streamNumber, m_pD3DVertexBuffer, 0,  stride );	
}


void Sophia::cRscVertexBuffer::SetStreamSourceFreq( UINT streamNumber,UINT setting )
{
	Graphics::m_pDevice->SetStreamSourceFreq(streamNumber,setting);	
}
