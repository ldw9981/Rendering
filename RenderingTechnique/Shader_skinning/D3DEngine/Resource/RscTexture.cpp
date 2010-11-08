#include "stdafx.h"
#include "RscTexture.h"
#include "../Log/Trace.h"
#include "ResourceMng.h"

cRscTexture::cRscTexture(void)
:m_pD3DTexture(NULL)
{	
	SetType(TEXTURE);
}


cRscTexture::~cRscTexture(void)
{
	//원래는 Release후에 호출하지만 Release사용하지않았으면 호출해준다.
	SAFE_RELEASE(m_pD3DTexture);
}


BOOL cRscTexture::Create()
{	
	if (!GetFilePath().empty())
	{	
		if (AddRef()==1)
		{
			HRESULT hResult=D3DXCreateTextureFromFile(m_pD3DDevice9,GetFilePath().c_str(),&m_pD3DTexture);
			if (!SUCCEEDED(hResult))
				return FALSE;

		}	
	}	
	else
	{
		if (AddRef()==1)
		{

		}
	}
	MakeUniqeKey();	
	return TRUE;	
}



void cRscTexture::Restore()
{

}

void cRscTexture::Free()
{
	SAFE_RELEASE(m_pD3DTexture);
	GetResourceMng()->EraseResource(GetUniqeKey());
	delete this;
}

void cRscTexture::ApplyStage( UINT stage )
{
	m_pD3DDevice9->SetTexture(stage,m_pD3DTexture);
}