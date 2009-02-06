#include "stdafx.h"
#include "RscTexture.h"
#include "../Log/Trace.h"
#include "ResourceMng.h"

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
	if (!GetFilePath().empty())
	{	
		if (AddRef()==1)
		{
			HRESULT hResult=D3DXCreateTextureFromFile(m_pD3DDevice,GetFilePath().c_str(),&m_pD3DTexture);
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
	ProcessMakeUniqueKey();	
	return TRUE;	
}



void cRscTexture::Restore()
{

}

void cRscTexture::Free()
{
	SAFE_RELEASE(m_pD3DTexture);
	GetResourceMng()->EraseResource(GetUniqueKey());
	delete this;
}

void cRscTexture::SetTexture( UINT stage )
{
	m_pD3DDevice->SetTexture(stage,m_pD3DTexture);
}

void cRscTexture::ProcessMakeUniqueKey()
{
	// 파일이름이 있으면 접두어_파일이름
	// 없으면 접두어_유니크번호
	wstring temp=L"TEXTURE_";	

	if (!GetFilePath().empty())
	{			
		temp += GetFilePath();		
	}
	else
	{
		WCHAR buffer[4];
		_itow_s(GetUniqueNumber(),buffer,4,10);
		temp += buffer;
	}
	SetUniqueKey(temp);
}

void cRscTexture::SetNullTexture( UINT stage )
{
	m_pD3DDevice->SetTexture(stage,NULL);
}