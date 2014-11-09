#include "StdAfx.h"
#include "MatrixStreamVertexBuffer.h"
#include "Graphics/Graphics.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"

using namespace Sophia;

MatrixStreamVertexBuffer::MatrixStreamVertexBuffer(void)
{
	m_pool=D3DPOOL_DEFAULT;
	m_BufferSize=sizeof(D3DXVECTOR3)*4*INSTANCING_MAX;
	m_usage = D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY;

	m_key.pIndexBuffer = NULL;
	m_key.pMaterial = NULL;
	m_key.pVertexBuffer = NULL;
	m_valid = false;
}


MatrixStreamVertexBuffer::~MatrixStreamVertexBuffer(void)
{
}

void MatrixStreamVertexBuffer::Free()
{
	SAFE_RELEASE(m_pD3DVertexBuffer);
	
	if (m_key.pIndexBuffer != NULL && m_key.pMaterial!=NULL && m_key.pVertexBuffer!=NULL)
	{
		cResourceMng::m_pInstance->EraseMatrixStreamVertexBuffer(m_key);
	}	
		
	delete this;
}
