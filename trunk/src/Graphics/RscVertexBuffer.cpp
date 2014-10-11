#include "stdafx.h"
#include "RscVertexBuffer.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"
#include "Graphics/Graphics.h"


using namespace Sophia;

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
	미리 설정한 버퍼 사이즈로 할당만 한다.
*/
BOOL cRscVertexBuffer::Create()
{
	assert(m_RefCounter>=0);
	if (m_RefCounter > 0 )
		return TRUE;

	HRESULT hResult;
	
	// ! VertexBuffer
	// 1) FVF를 설정해서 필요한크기만큼 VertexBuffer를 만든다.	^
	hResult=Graphics::m_pDevice->CreateVertexBuffer( 
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
	if (!GetUniqueKey().empty())
	{
		cResourceMng::m_pInstance->EraseRscVertexBuffer(GetUniqueKey());
	}	
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



void cRscVertexBuffer::SetStreamSource(UINT streamNumber,  UINT stride )
{
	Graphics::m_pDevice->SetStreamSource( streamNumber, m_pD3DVertexBuffer, 0,  stride );	
}


void Sophia::cRscVertexBuffer::SetStreamSourceFreq( UINT streamNumber,UINT setting )
{
	Graphics::m_pDevice->SetStreamSourceFreq(streamNumber,setting);	
}
