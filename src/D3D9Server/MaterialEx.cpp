#include "StdAfx.h"
#include "MaterialEx.h"
#include "D3D9Server/RscTexture.h"
#include "Foundation/Define.h"

Material::Material(void)
{
	Ambient		= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	Diffuse		= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	Emissive	= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);	
	Specular	= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);

	Power		= 0.0f;
	Multiply	= 0.0f;
	Transparency = 0.0f;


	// 텍스쳐의 생성은 리소스매니져를 통해 생성한다.
	m_pMapDiffuse = NULL;
	m_pMapBump = NULL;
	m_pMapRefract = NULL;
}

Material::Material( const Material &Other )
{
	Ambient		= Other.Ambient;
	Diffuse		= Other.Diffuse;
	Emissive	= Other.Emissive;
	Power		= Other.Power;
	Specular	= Other.Specular;
	Multiply	= Other.Multiply;
	Transparency = Other.Transparency;	


	m_pMapDiffuse = Other.m_pMapDiffuse;		
	if (m_pMapDiffuse)
	{
		m_pMapDiffuse->AddRef();					
	}			

	m_pMapBump = Other.m_pMapBump;
	if (m_pMapBump)
	{
		m_pMapBump->AddRef();					
	}

	m_pMapRefract = Other.m_pMapRefract;
	if (m_pMapRefract)
	{
		m_pMapRefract->AddRef();					
	}
}
Material::~Material(void)
{
	SAFE_RELEASE(m_pMapDiffuse);		
	SAFE_RELEASE(m_pMapBump);
	SAFE_RELEASE(m_pMapRefract);
}

Material& Material::operator =(const Material &Other)
{
	Ambient		= Other.Ambient;
	Diffuse		= Other.Diffuse;
	Emissive	= Other.Emissive;
	Power		= Other.Power;
	Specular	= Other.Specular;
	Multiply	= Other.Multiply;
	Transparency = Other.Transparency;


	if (m_pMapDiffuse)
	{
		m_pMapDiffuse->Release();					
	}
	m_pMapDiffuse = Other.m_pMapDiffuse;	
	if (m_pMapDiffuse)
	{
		m_pMapDiffuse->AddRef();					
	}	

	if (m_pMapBump)
	{
		m_pMapBump->Release();					
	}
	m_pMapBump = Other.m_pMapBump;	
	if (m_pMapBump)
	{
		m_pMapBump->AddRef();					
	}	
	
	if (m_pMapRefract)
	{
		m_pMapRefract->Release();					
	}
	m_pMapRefract = Other.m_pMapRefract;	
	if (m_pMapRefract)
	{
		m_pMapRefract->AddRef();					
	}
	return *this;
}

void Material::SetMapDiffuse( cRscTexture* val )
{
	if (m_pMapDiffuse)
	{
		m_pMapDiffuse->Release();
	}			
	m_pMapDiffuse = val;
	if (m_pMapDiffuse)
	{
		m_pMapDiffuse->AddRef();
	}
}

cRscTexture* Material::GetMapDiffuse() const
{
	return m_pMapDiffuse;
}

cRscTexture* Material::GetMapBump() const
{
	return m_pMapBump;
}

void Material::SetMapBump( cRscTexture* val )
{	
	if (m_pMapBump)
	{
		m_pMapBump->Release();
	}			
	m_pMapBump = val;
	if (m_pMapBump)
	{
		m_pMapBump->AddRef();
	}
}

cRscTexture* Material::GetMapRefract() const
{
	return m_pMapRefract;
}

void Material::SetMapRefract( cRscTexture* val )
{
	if (m_pMapRefract)
	{
		m_pMapRefract->Release();
	}			
	m_pMapRefract = val;
	if (m_pMapRefract)
	{
		m_pMapRefract->AddRef();
	}
}