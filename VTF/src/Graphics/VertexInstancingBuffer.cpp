#include "StdAfx.h"
#include "VertexInstancingBuffer.h"
#include "Graphics/Graphics.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"

using namespace Sophia;

VertexInstancingBuffer::VertexInstancingBuffer(void)
{
	m_pool=D3DPOOL_MANAGED;
	m_usage = D3DUSAGE_WRITEONLY;
	m_key = NULL;
}



VertexInstancingBuffer::~VertexInstancingBuffer(void)
{
}
void VertexInstancingBuffer::Free()
{
	SAFE_RELEASE(m_pD3DVertexBuffer);

	if (m_key!=NULL)
	{
		cResourceMng::m_pInstance->EraseVertexInstancingBuffer(m_key);
	}	

	delete this;
}