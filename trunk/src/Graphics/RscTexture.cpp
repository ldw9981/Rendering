#include "stdafx.h"
#include "RscTexture.h"
#include "Foundation/Trace.h"
#include "Resource/ResourceMng.h"
#include "Foundation/Define.h"
#include "Graphics/Graphics.h"

namespace Sophia
{


cRscTexture::cRscTexture(void)
:m_pD3DTexture(NULL)
{	
	m_width = 0;
	m_height = 0;
	m_levels = 1;
	m_pool = D3DPOOL_MANAGED;
	m_usage = D3DUSAGE_DYNAMIC;
	m_format = D3DFMT_R32F;
}


cRscTexture::~cRscTexture(void)
{
	//원래는 Release후에 호출하지만 Release사용하지않았으면 호출해준다.
	
	SAFE_RELEASE(m_pD3DTexture);
}


BOOL cRscTexture::Create()
{	
	if (m_RefCounter > 0 )
		return TRUE;

	HRESULT hResult;
	if (!m_filePath.empty())
	{
		hResult = D3DXCreateTextureFromFile(Graphics::m_pDevice,m_filePath.c_str(),&m_pD3DTexture);
	}
	else
	{
		hResult = Graphics::m_pDevice->CreateTexture(m_width,m_height,m_levels,m_usage,m_format ,m_pool, &m_pD3DTexture, NULL );
	}

	return SUCCEEDED(hResult);		
}



void cRscTexture::Restore()
{

}

void cRscTexture::Free()
{
	SAFE_RELEASE(m_pD3DTexture);
	if (!GetUniqueKey().empty())
	{
		cResourceMng::m_pInstance->EraseRscTexture(GetUniqueKey());
	}	
	delete this;
}

void cRscTexture::SetTexture( UINT stage )
{
	Graphics::m_pDevice->SetTexture(stage,m_pD3DTexture);
}


void cRscTexture::SetNullTexture( UINT stage )
{
	Graphics::m_pDevice->SetTexture(stage,NULL);
}

}