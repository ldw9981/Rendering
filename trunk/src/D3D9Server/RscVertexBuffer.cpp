#include "stdafx.h"
#include "RscVertexBuffer.h"
#include "Foundation/Define.h"


cRscVertexBuffer::cRscVertexBuffer(void)
{
	m_pD3DVertexBuffer=NULL;
	m_Type=D3DPOOL_DEFAULT;
	m_nVerties=0;
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
	GetResourceMng()->EraseResource(GetUniqueKey());
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

void cRscVertexBuffer::ProcessMakeUniqueKey()
{
	// 파일이름이 있으면 접두어_파일이름
	// 없으면 접두어_유니크번호
	string temp="VERTEXBUFFER_";	

	if (!GetFilePath().empty())
	{			
		temp += GetFilePath();		
	}
	else
	{
		char buffer[4];
		_itoa_s(GetUniqueNumber(),buffer,4,10);
		temp += buffer;
	}
	SetUniqueKey(temp);
}

void cRscVertexBuffer::SetStreamSource( UINT stride )
{
	m_pD3DDevice->SetStreamSource( 0, m_pD3DVertexBuffer, 0,  stride );	
}
