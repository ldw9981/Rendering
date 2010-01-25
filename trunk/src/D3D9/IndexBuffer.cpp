#include "IndexBuffer.h"
#include "../Foundation/Define.h"
#include <assert.h>

IndexBuffer::IndexBuffer(void)
{	
	m_pD3DIndexBuffer=NULL;
	m_Type=D3DPOOL_DEFAULT;
}

IndexBuffer::~IndexBuffer(void)
{
	
}

BOOL IndexBuffer::Create()
{
	assert(m_RefCounter>=0);
	if (AddRef()!=1)
		return TRUE;

	HRESULT hResult;		
	// ! IndexBuffer
	// 1) �ʿ��� Index���� ��ŭ�� ũ��� IndexBuffer�� �����.
	hResult=m_SharedLPDIRECT3DDEVICE9->CreateIndexBuffer( 
		m_BufferSize,
		0,
		D3DFMT_INDEX16,
		m_Type,
		&m_pD3DIndexBuffer,
		NULL);
	assert(hResult==S_OK);
	
	return TRUE;
}


void IndexBuffer::Restore()
{
	
}

void IndexBuffer::Free()
{	
	SAFE_RELEASE(m_pD3DIndexBuffer);
	m_SharedResourceMng.EraseResource(GetUniqueKey());
	delete this;
}

void* IndexBuffer::Lock()
{
	void *pIndices;
	m_pD3DIndexBuffer->Lock( 0, m_BufferSize, (void**)&pIndices,0);		
	return pIndices;
}

VOID IndexBuffer::Unlock()
{
	m_pD3DIndexBuffer->Unlock();	
}

void IndexBuffer::ProcessMakeUniqueKey()
{
	// �����̸��� ������ ���ξ�_�����̸�
	// ������ ���ξ�_����ũ��ȣ
	wstring temp=L"INDEXBUFFER_";	

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

void IndexBuffer::SetIndices()
{
	m_SharedLPDIRECT3DDEVICE9->SetIndices(m_pD3DIndexBuffer); 
}