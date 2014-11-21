#include "StdAfx.h"
#include "IndexInstancingBuffer.h"
#include "Graphics/Vertex.h"
#include "Graphics/Graphics.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"

using namespace Sophia;
IndexInstancingBuffer::IndexInstancingBuffer(void)
{		
	m_Type=D3DPOOL_MANAGED;
	m_usage = D3DUSAGE_WRITEONLY;
	m_key = NULL;
	m_format = D3DFMT_INDEX32;
	m_valid = false;
}


IndexInstancingBuffer::~IndexInstancingBuffer(void)
{
}

void IndexInstancingBuffer::Free()
{
	SAFE_RELEASE(m_pD3DIndexBuffer);

	if (m_key!=NULL)
	{
		cResourceMng::m_pInstance->EraseIndexInstancingBuffer(m_key);
	}	

	delete this;
}
