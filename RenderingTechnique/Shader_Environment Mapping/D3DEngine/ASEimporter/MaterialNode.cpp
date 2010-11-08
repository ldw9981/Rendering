#include "StdAfx.h"
#include "MaterialNode.h"
#include "../Resource/RscTexture.h"

cMaterialNode::cMaterialNode(void)
{
	Ambient		= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	Diffuse		= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	Emissive	= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);	
	Specular	= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);

	Power		= 0.0f;
	Multiply	= 0.0f;
	Transparency = 0.0f;
	m_pTexture = NULL;
}

cMaterialNode::cMaterialNode( const cMaterialNode &Other )
{
	Ambient		= Other.Ambient;
	Diffuse		= Other.Diffuse;
	Emissive	= Other.Emissive;
	Power		= Other.Power;
	Specular	= Other.Specular;
	Multiply	= Other.Multiply;
	Transparency = Other.Transparency;	

	m_arrSubMaterial = Other.m_arrSubMaterial;

	m_pTexture = Other.m_pTexture;	
	if (m_pTexture!=NULL)
	{
		m_pTexture->AddReference();					
	}	
}
cMaterialNode::~cMaterialNode(void)
{
	if (m_pTexture!=NULL)
	{
		m_pTexture->Release();
	}	
}

cMaterialNode& cMaterialNode::operator =(const cMaterialNode &Other)
{
	Ambient		= Other.Ambient;
	Diffuse		= Other.Diffuse;
	Emissive	= Other.Emissive;
	Power		= Other.Power;
	Specular	= Other.Specular;
	Multiply	= Other.Multiply;
	Transparency = Other.Transparency;

	m_arrSubMaterial = Other.m_arrSubMaterial;
	m_pTexture = Other.m_pTexture;	
	if (m_pTexture!=NULL)
	{
		m_pTexture->AddReference();					
	}	
	return *this;
}

void cMaterialNode::InsertSubMaterial( cMaterialNode& SubMatrial )
{
	m_arrSubMaterial.push_back(SubMatrial);
}



cMaterialNode* cMaterialNode::GetSubMaterial( UINT index )
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

