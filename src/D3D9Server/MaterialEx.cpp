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

	// �ؽ����� ������ ���ҽ��Ŵ����� ���� �����Ѵ�.
	m_pMapDiffuse = NULL;
	m_pMapNormal = NULL;
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

	m_pMapDiffuse = NULL;
	m_pMapNormal = NULL;
	m_pMapRefract = NULL;
	SetMapDiffuse(Other.m_pMapDiffuse);
	SetMapNormal(Other.m_pMapNormal);
	SetMapRefract(Other.m_pMapRefract);
}
Material::~Material(void)
{
	SAFE_RELEASE(m_pMapDiffuse);		
	SAFE_RELEASE(m_pMapNormal);
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
	
	SetMapDiffuse(Other.m_pMapDiffuse);
	SetMapNormal(Other.m_pMapNormal);
	SetMapRefract(Other.m_pMapRefract);
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

	if (m_pMapDiffuse)
	{
		index_renderer_queue_.set(DIFFUSE);
	}
	else
	{
		index_renderer_queue_.reset(DIFFUSE);	
	}
}

cRscTexture* Material::GetMapDiffuse() const
{
	return m_pMapDiffuse;
}

cRscTexture* Material::GetMapNormal() const
{
	return m_pMapNormal;
}

void Material::SetMapNormal( cRscTexture* val )
{		
	if (m_pMapNormal)
	{
		m_pMapNormal->Release();
	}			
	m_pMapNormal = val;
	if (m_pMapNormal)
	{
		m_pMapNormal->AddRef();
	}

	if (m_pMapNormal)
	{
		index_renderer_queue_.set(NORMAL);
	}
	else
	{
		index_renderer_queue_.reset(NORMAL);	
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

	if (m_pMapRefract)
	{
		index_renderer_queue_.set(REFRACT);
	}
	else
	{
		index_renderer_queue_.reset(REFRACT);	
	}
}