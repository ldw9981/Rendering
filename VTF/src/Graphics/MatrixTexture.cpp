#include "StdAfx.h"
#include "MatrixTexture.h"
#include "Resource/ResourceMng.h"
#include "Foundation/Define.h"
#include "Scene/MeshNode.h"
using namespace Sophia;
MatrixTexture::MatrixTexture(void)
{
	m_levels = 1;
	m_pool = D3DPOOL_DEFAULT;
	m_usage = D3DUSAGE_DYNAMIC;
	m_format = D3DFMT_A32B32G32R32F ;

	m_key.pIndexBuffer = NULL;
	m_key.pMaterial = NULL;
	m_key.pVertexBuffer = NULL;
	m_valid = false;
}


MatrixTexture::~MatrixTexture(void)
{
}

void MatrixTexture::Free()
{
	SAFE_RELEASE(m_pD3DTexture);

	if (m_key.pIndexBuffer != NULL && m_key.pMaterial!=NULL && m_key.pVertexBuffer!=NULL)
	{
		cResourceMng::m_pInstance->EraseMatrixTexture(m_key);
	}	
	delete this;
}


void Sophia::MatrixTexture::SetSize( DWORD val )
{
	m_size = val;
	m_width = val;
	m_height = val;
}
