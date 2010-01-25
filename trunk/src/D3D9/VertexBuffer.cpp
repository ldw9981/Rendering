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
	�̸� ������ ���� ������� �Ҵ縸 �Ѵ�.
*/
BOOL VertexBuffer::Create()
{
	assert(m_RefCounter>=0);
	if (AddRef()!=1)
		return TRUE;

	HRESULT hResult;
	
	// ! VertexBuffer
	// 1) FVF�� �����ؼ� �ʿ���ũ�⸸ŭ VertexBuffer�� �����.	^
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
	// �����̸��� ������ ���ξ�_�����̸�
	// ������ ���ξ�_����ũ��ȣ
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