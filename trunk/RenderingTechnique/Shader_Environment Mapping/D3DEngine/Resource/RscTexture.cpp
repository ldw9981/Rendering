#include "stdafx.h"
#include "RscTexture.h"
#include "../Log/Trace.h"

cRscTexture::cRscTexture(void)
:m_pD3DTexture(NULL)
{	
	SetType(TEXTURE);
}


cRscTexture::~cRscTexture(void)
{
	Destroy();
}


BOOL cRscTexture::Create()
{	
	if (!GetFilePath().empty())
	{	
		
		HRESULT hResult=D3DXCreateTextureFromFile(m_pD3DDevice9,GetFilePath().c_str(),&m_pD3DTexture);
		if (!SUCCEEDED(hResult))
			return FALSE;
	}	
	else
	{
		
	}

	AddReference();
	MakeUniqeKey();
	return TRUE;	
}

BOOL cRscTexture::Destroy()
{
	if (Release()==0)
	{
		return TRUE;
	}		
	return FALSE;
}

void cRscTexture::Restore()
{

}

void cRscTexture::Free()
{
	SAFE_RELEASE(m_pD3DTexture);
}