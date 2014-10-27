#include "StdAfx.h"
#include "VertexStream.h"
#include "Graphics/Graphics.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"

using namespace Sophia;

VertexStream::VertexStream(void)
{
	m_pool=D3DPOOL_DEFAULT;
	m_usage = D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY;
	//m_BufferSize =sizeof(NORMALVERTEX)*INSTANCING_MAX;
	m_key.pIndexBuffer = NULL;
	m_key.pMaterial = NULL;
	m_key.pVertexBuffer = NULL;
}



VertexStream::~VertexStream(void)
{
}
void VertexStream::Free()
{
	SAFE_RELEASE(m_pD3DVertexBuffer);

	if (m_key.pIndexBuffer != NULL && m_key.pMaterial!=NULL && m_key.pVertexBuffer!=NULL)
	{
		cResourceMng::m_pInstance->EraseVertexTransformationTexture(m_key);
	}	

	delete this;
}