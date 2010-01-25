#include "Texture.h"
#include "../Foundation/Trace.h"
#include "../Foundation/Define.h"
#include "../Foundation/ResourceMng.h"
#include <d3dx9tex.h>

Texture::Texture(void)
:m_pD3DTexture(NULL)
{	
	
}


Texture::~Texture(void)
{
	//원래는 Release후에 호출하지만 Release사용하지않았으면 호출해준다.
	
	SAFE_RELEASE(m_pD3DTexture);
}


BOOL Texture::Create()
{	
	if (!GetFilePath().empty())
	{	
		if (AddRef()==1)
		{
			HRESULT hResult=D3DXCreateTextureFromFile(m_SharedLPDIRECT3DDEVICE9,GetFilePath().c_str(),&m_pD3DTexture);
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



void Texture::Restore()
{

}

void Texture::Free()
{
	SAFE_RELEASE(m_pD3DTexture);
	m_SharedResourceMng.EraseResource(GetUniqueKey());
	delete this;
}

void Texture::SetTexture( UINT stage )
{
	m_SharedLPDIRECT3DDEVICE9->SetTexture(stage,m_pD3DTexture);
}

void Texture::ProcessMakeUniqueKey()
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

void Texture::SetNullTexture( UINT stage )
{
	m_SharedLPDIRECT3DDEVICE9->SetTexture(stage,NULL);
}