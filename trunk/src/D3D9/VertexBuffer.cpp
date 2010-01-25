#include "VertexBuffer.h"
#include "../Foundation/Define.h"
#include <assert.h>



VertexBuffer::VertexBuffer(void)
{
	m_pD3DVertexBuffer=NULL;
	m_Type=D3DPOOL_DEFAULT;
}

VertexBuffer::~VertexBuffer(void)
{
	
}


/*
	미리 설정한 버퍼 사이즈로 할당만 한다.
*/
BOOL VertexBuffer::Create()
{
	assert(m_RefCounter>=0);
	if (AddRef()!=1)
		return TRUE;

	HRESULT hResult;
	
	// ! VertexBuffer
	// 1) FVF를 설정해서 필요한크기만큼 VertexBuffer를 만든다.	^
	hResult=m_SharedLPDIRECT3DDEVICE9->CreateVertexBuffer( 
			m_BufferSize,
			0,
			0,
			m_Type,
			&m_pD3DVertexBuffer,
			NULL);

	assert(hResult==S_OK);	
	return TRUE;
}



void VertexBuffer::Free()
{
	SAFE_RELEASE(m_pD3DVertexBuffer);
	m_SharedResourceMng.EraseResource(GetUniqueKey());
	delete this;
}

void VertexBuffer::Restore()
{

}



void* VertexBuffer::Lock()
{
	void *pVertices=NULL;
	m_pD3DVertexBuffer->Lock( 0, m_BufferSize, (void**)&pVertices, 0 ) ;
	return pVertices;
}

void VertexBuffer::Unlock()
{
	m_pD3DVertexBuffer->Unlock();
}

void VertexBuffer::ProcessMakeUniqueKey()
{
	// 파일이름이 있으면 접두어_파일이름
	// 없으면 접두어_유니크번호
	wstring temp=L"VERTEXBUFFER_";	

	if (!GetFilePath().empty())
	{			
		temp += GetFilePath();		
	}
	else
	{
		WCHAR buffer[4];
		_itow_s(GetUniqueNumber(),buffer,4,10);
		temp += buffer;
	}
	SetUniqueKey(temp);
}

void VertexBuffer::SetStreamSource( UINT stride )
{
	m_SharedLPDIRECT3DDEVICE9->SetStreamSource( 0, m_pD3DVertexBuffer, 0,  stride );	
}