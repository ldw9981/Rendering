#include "StdAfx.h"
#include "IndexStreamVertexBuffer.h"
#include "Graphics/Vertex.h"
#include "Graphics/Graphics.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"

using namespace Sophia;
IndexStreamVertexBuffer::IndexStreamVertexBuffer(void)
{
	m_pool=D3DPOOL_DEFAULT;
	m_BufferSize=sizeof(INSTANCEVERTEX)*INSTANCING_MAX;
	m_usage = D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY;
	
	m_key.pIndexBuffer = NULL;
	m_key.pMaterial = NULL;
	m_key.pVertexBuffer = NULL;
	m_valid = false;
}


IndexStreamVertexBuffer::~IndexStreamVertexBuffer(void)
{
}

void IndexStreamVertexBuffer::Free()
{
	SAFE_RELEASE(m_pD3DVertexBuffer);

	if (m_key.pIndexBuffer != NULL && m_key.pMaterial!=NULL && m_key.pVertexBuffer!=NULL)
	{
		cResourceMng::m_pInstance->EraseIndexStreamVertexBuffer(m_key);
	}	

	delete this;
}
