#include "StdAfx.h"
#include "MaterialEx.h"
#include "D3D9Server/RscTexture.h"
#include "Foundation/Define.h"

cMaterialEx::cMaterialEx(void)
{
	Ambient		= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	Diffuse		= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	Emissive	= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);	
	Specular	= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);

	Power		= 0.0f;
	Multiply	= 0.0f;
	Transparency = 0.0f;


	// 텍스쳐의 생성은 리소스매니져를 통해 생성한다.
	m_pTexture = NULL;
}

cMaterialEx::cMaterialEx( const cMaterialEx &Other )
{
	Ambient		= Other.Ambient;
	Diffuse		= Other.Diffuse;
	Emissive	= Other.Emissive;
	Power		= Other.Power;
	Specular	= Other.Specular;
	Multiply	= Other.Multiply;
	Transparency = Other.Transparency;	


	m_pTexture = Other.m_pTexture;	
	
	if (m_pTexture)
	{
		m_pTexture->AddRef();					
	}			
}
cMaterialEx::~cMaterialEx(void)
{
	SAFE_RELEASE(m_pTexture);		
}

cMaterialEx& cMaterialEx::operator =(const cMaterialEx &Other)
{
	Ambient		= Other.Ambient;
	Diffuse		= Other.Diffuse;
	Emissive	= Other.Emissive;
	Power		= Other.Power;
	Specular	= Other.Specular;
	Multiply	= Other.Multiply;
	Transparency = Other.Transparency;


	if (m_pTexture)
	{
		m_pTexture->Release();					
	}
	m_pTexture = Other.m_pTexture;	
	if (m_pTexture)
	{
		m_pTexture->AddRef();					
	}	
	
	return *this;
}

void cMaterialEx::SetRscTexture( cRscTexture* val )
{
	if (m_pTexture)
	{
		m_pTexture->Release();
	}			
	m_pTexture = val;
	if (m_pTexture)
	{
		m_pTexture->AddRef();
	}
}