#include "StdAfx.h"
#include "InstanceDataBuffer.h"
#include "Graphics/Vertex.h"
#include "Graphics/Graphics.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"

using namespace Sophia;
InstanceDataBuffer::InstanceDataBuffer(void)
{
	m_pool=D3DPOOL_MANAGED;
	m_BufferSize=sizeof(INSTANCEDATA)*INSTANCING_MAX;
	m_usage = D3DUSAGE_WRITEONLY;

	m_key = NULL;
}


InstanceDataBuffer::~InstanceDataBuffer(void)
{
}

void InstanceDataBuffer::Free()
{
	SAFE_RELEASE(m_pD3DVertexBuffer);

	if (m_key!=NULL)
	{
		cResourceMng::m_pInstance->EraseInstanceDataBuffer(m_key);
	}	

	delete this;
}
