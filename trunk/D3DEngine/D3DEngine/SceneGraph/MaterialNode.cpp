#include "StdAfx.h"
#include "MaterialNode.h"
#include "../Resource/RscTexture.h"

cMaterialEx::cMaterialEx(void)
{
	Ambient		= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	Diffuse		= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	Emissive	= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);	
	Specular	= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);

	Power		= 0.0f;
	Alpha		= 0.0f;

	// 텍스쳐의 생성은 리소스매니져를 통해 생성한다.
	m_pDiffuseRscTexture = NULL;
	m_pBumpRscTexture = NULL;
}

cMaterialEx::cMaterialEx( const cMaterialEx &Other )
{
	Ambient		= Other.Ambient;
	Diffuse		= Other.Diffuse;
	Emissive	= Other.Emissive;
	Power		= Other.Power;
	Specular	= Other.Specular;
	SpecularMultiplier = Other.SpecularMultiplier;

	Alpha		= Other.Alpha;	
	
	m_arrSubMaterial = Other.m_arrSubMaterial;
	
	m_pDiffuseRscTexture = Other.m_pDiffuseRscTexture;	
	if (m_pDiffuseRscTexture!=NULL)
	{
		m_pDiffuseRscTexture->AddRef();					
	}

	m_pBumpRscTexture = Other.m_pBumpRscTexture;
	if (m_pBumpRscTexture!=NULL)
	{
		m_pBumpRscTexture->AddRef();					
	}
}
cMaterialEx::~cMaterialEx(void)
{
	SAFE_RELEASE(m_pDiffuseRscTexture);		
}

cMaterialEx& cMaterialEx::operator =(const cMaterialEx &Other)
{
	Ambient		= Other.Ambient;
	Diffuse		= Other.Diffuse;
	Emissive	= Other.Emissive;
	Power		= Other.Power;
	Specular	= Other.Specular;
	SpecularMultiplier = Other.SpecularMultiplier;
	Alpha		= Other.Alpha;
	
	m_arrSubMaterial = Other.m_arrSubMaterial;

	m_pDiffuseRscTexture = Other.m_pDiffuseRscTexture;	
	if (m_pDiffuseRscTexture!=NULL)
	{
		m_pDiffuseRscTexture->AddRef();					
	}	

	m_pBumpRscTexture = Other.m_pBumpRscTexture;
	if (m_pBumpRscTexture!=NULL)
	{
		m_pBumpRscTexture->AddRef();					
	}	

	
	return *this;
}

void cMaterialEx::InsertSubMaterial( cMaterialEx& SubMatrial )
{
	m_arrSubMaterial.push_back(SubMatrial);
}



cMaterialEx* cMaterialEx::GetSubMaterial( UINT index )
{
	if (!m_arrSubMaterial.empty())
	{
		if( index >= m_arrSubMaterial.size())
		{
			return NULL;
		}
		return &m_arrSubMaterial[index];
	}
	return NULL;
}


cMaterialEx2::cMaterialEx2(void)
{
	Ambient		= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	Diffuse		= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	Emissive	= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);	
	Specular	= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);

	Power		= 0.0f;
	Alpha		= 0.0f;

	// 텍스쳐의 생성은 리소스매니져를 통해 생성한다.
	m_pDiffuseRscTexture = NULL;
	m_pBumpRscTexture = NULL;
	m_pOpacityRscTexture = NULL;
}


cMaterialEx2::~cMaterialEx2(void)
{
	SAFE_RELEASE(m_pDiffuseRscTexture);		
	SAFE_RELEASE(m_pBumpRscTexture);		
	SAFE_RELEASE(m_pOpacityRscTexture);		
}


void cMaterialEx2::InsertSubMaterial( cMaterialEx2* SubMatrial )
{
	m_arrSubMaterial.push_back(SubMatrial);
}



cMaterialEx2* cMaterialEx2::GetSubMaterial( UINT index )
{
	if (!m_arrSubMaterial.empty())
	{
		if( index >= m_arrSubMaterial.size())
		{
			return NULL;
		}
		return &m_arrSubMaterial[index];
	}
	return NULL;
}

