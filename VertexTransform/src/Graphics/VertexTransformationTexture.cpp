#include "StdAfx.h"
#include "VertexTransformationTexture.h"
#include "Graphics/Graphics.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"

using namespace Sophia;

VertexTransformationTexture::VertexTransformationTexture(void)
{
	m_key.pIndexBuffer = NULL;
	m_key.pMaterial = NULL;
	m_key.pVertexBuffer = NULL;
	m_levels = 1;
	m_format = D3DFMT_A32B32G32R32F;
	m_pool =  D3DPOOL_DEFAULT;
	m_usage = D3DUSAGE_RENDERTARGET;
	m_width = 512;
	m_height = 512;
}


VertexTransformationTexture::~VertexTransformationTexture(void)
{
}
void VertexTransformationTexture::Free()
{
	SAFE_RELEASE(m_pD3DTexture);

	if (m_key.pIndexBuffer != NULL && m_key.pMaterial!=NULL && m_key.pVertexBuffer!=NULL)
	{
		cResourceMng::m_pInstance->EraseVertexTransformationTexture(m_key);
	}	

	delete this;
}