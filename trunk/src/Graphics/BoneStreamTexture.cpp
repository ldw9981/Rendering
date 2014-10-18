#include "StdAfx.h"
#include "BoneStreamTexture.h"
#include "Resource/ResourceMng.h"
#include "Foundation/Define.h"

using namespace Sophia;
BoneStreamTexture::BoneStreamTexture(void)
{
	m_width = 64*4;
	m_height = 128;
	m_levels = 1;
	m_pool = D3DPOOL_DEFAULT;
	m_usage = D3DUSAGE_DYNAMIC;
	m_format = D3DFMT_A32B32G32R32F ;

	m_key.pIndexBuffer = NULL;
	m_key.pMaterial = NULL;
	m_key.pVertexBuffer = NULL;
	m_valid = false;
}


BoneStreamTexture::~BoneStreamTexture(void)
{
}

void BoneStreamTexture::Free()
{
	SAFE_RELEASE(m_pD3DTexture);

	if (m_key.pIndexBuffer != NULL && m_key.pMaterial!=NULL && m_key.pVertexBuffer!=NULL)
	{
		cResourceMng::m_pInstance->EraseBoneStreamTexture(m_key);
	}	
	delete this;
}
