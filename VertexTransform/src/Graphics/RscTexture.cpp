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
	m_format = D3DFMT_A32B32G32R32F ;
}


cRscTexture::~cRscTexture(void)
{
	//������ Release�Ŀ� ȣ�������� Release��������ʾ����� ȣ�����ش�.
	
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
	assert(SUCCEEDED(hResult));
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

void cRscTexture::Lock( D3DLOCKED_RECT* pLockRect,DWORD flags )
{
	FAILED(m_pD3DTexture->LockRect(0,pLockRect,NULL,flags));
}

void cRscTexture::Unlock()
{
	FAILED(m_pD3DTexture->UnlockRect(0));
}

}