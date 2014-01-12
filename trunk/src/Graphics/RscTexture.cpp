#include "stdafx.h"
#include "RscTexture.h"
#include "Foundation/Trace.h"
#include "Resource/ResourceMng.h"
#include "Foundation/Define.h"
#include "Graphics/Graphics.h"

cRscTexture::cRscTexture(void)
:m_pD3DTexture(NULL)
{	
	
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

	if (m_filePath.empty())
		return FALSE;

	HRESULT hResult=D3DXCreateTextureFromFile(Graphics::m_pDevice,m_filePath.c_str(),&m_pD3DTexture);
	return SUCCEEDED(hResult);		
}



void cRscTexture::Restore()
{

}

void cRscTexture::Free()
{
	SAFE_RELEASE(m_pD3DTexture);
	cResourceMng::m_pResourceMng->EraseRscTexture(GetUniqueKey());
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