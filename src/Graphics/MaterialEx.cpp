#include "StdAfx.h"
#include "MaterialEx.h"
#include "Graphics/RscTexture.h"
#include "Foundation/Define.h"
#include "Foundation/StringUtil.h"
#include "Resource/ResourceMng.h"
#include "Foundation/EnvironmentVariable.h"
namespace Sophia
{

#define MATERIAL_LASTEST 4


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
	m_pMapNormal = NULL;
	m_pMapLight = NULL;
	m_pMapOpacity = NULL;
	m_pMapSpecular = NULL;
	AlphaBlend = false;
	AlphaTestEnable = false;
	AlphaTestRef = 0;
	//m_pMapRefract = NULL;
	
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
	AlphaBlend = Other.AlphaBlend;
	AlphaTestEnable = Other.AlphaTestEnable;
	AlphaTestRef = Other.AlphaTestRef;

	m_pMapDiffuse = NULL;
	m_pMapNormal = NULL;
	m_pMapLight = NULL;
	m_pMapOpacity = NULL;
	m_pMapSpecular = NULL;

	SetMapDiffuse(Other.m_pMapDiffuse);
	SetMapNormal(Other.m_pMapNormal);
	SetMapLight(Other.m_pMapLight);
	SetMapOpacity(Other.m_pMapOpacity);
	SetMapSpecular(Other.m_pMapSpecular);
	
}
Material::~Material(void)
{
	SAFE_RELEASE(m_pMapDiffuse);		
	SAFE_RELEASE(m_pMapNormal);
	SAFE_RELEASE(m_pMapLight);
	SAFE_RELEASE(m_pMapOpacity);
	SAFE_RELEASE(m_pMapSpecular);
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
	AlphaBlend = Other.AlphaBlend;
	AlphaTestEnable = Other.AlphaTestEnable;
	AlphaTestRef = Other.AlphaTestRef;
	
	SetMapDiffuse(Other.m_pMapDiffuse);
	SetMapNormal(Other.m_pMapNormal);
	SetMapLight(Other.m_pMapLight);
	SetMapOpacity(Other.m_pMapOpacity);
	SetMapSpecular(Other.m_pMapSpecular);
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

cRscTexture* Material::GetMapLight() const
{
	return m_pMapLight;
}

void Material::SetMapLight( cRscTexture* val )
{
	if (m_pMapLight)
	{
		m_pMapLight->Release();
	}	
	m_pMapLight = val;
	if (m_pMapLight)
	{		
		m_pMapLight->AddRef();
	}	

	if (m_pMapLight)
	{
		index_renderer_queue_.set(LIGHT);
	}
	else
	{
		index_renderer_queue_.reset(LIGHT);	
	}
}

cRscTexture* Material::GetMapOpacity() const
{
	return m_pMapOpacity;
}

void Material::SetMapOpacity( cRscTexture* val )
{
	if (m_pMapOpacity)
	{
		m_pMapLight->Release();
	}	
	m_pMapOpacity = val;
	if (m_pMapOpacity)
	{		
		m_pMapOpacity->AddRef();
	}	

	if (m_pMapOpacity)
	{
		index_renderer_queue_.set(OPACITY);
	}
	else
	{
		index_renderer_queue_.reset(OPACITY);	
	}
}

cRscTexture* Material::GetMapSpecular() const
{
	return m_pMapSpecular;
}

void Material::SetMapSpecular( cRscTexture* val )
{
	if (m_pMapSpecular)
	{
		m_pMapSpecular->Release();
	}	
	m_pMapSpecular = val;
	if (m_pMapSpecular)
	{		
		m_pMapSpecular->AddRef();
	}	

	if (m_pMapSpecular)
	{
		index_renderer_queue_.set(SPECULAR);
	}
	else
	{
		index_renderer_queue_.reset(SPECULAR);	
	}
}

void Material::SerializeIn( std::ifstream& stream )
{
	unsigned short ver = 0;
	stream.read((char*)&ver,sizeof(ver));
	assert(ver<=MATERIAL_LASTEST);

	unsigned char type = 0;
	std::string fileName;
	type = 0;
	stream.read((char*)&type,sizeof(type));
	assert(type<MAX);

	fileName.clear();
	ReadString(stream,fileName);
	if (fileName.length()!=0)
	{
		std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
		std::string strFullPath = strDataPath;
		strFullPath += fileName;
		cRscTexture* pRscTexture= cResourceMng::m_pInstance->CreateRscTexture(strFullPath.c_str());
		assert(pRscTexture!=NULL);
		if(pRscTexture)
			SetMapDiffuse(pRscTexture);
	}

	type = 0;
	stream.read((char*)&type,sizeof(type));
	assert(type<MAX);
	fileName.clear();
	ReadString(stream,fileName);
	if (fileName.length()!=0)
	{
		std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
		std::string strFullPath = strDataPath;
		strFullPath += fileName;
		cRscTexture* pRscTexture= cResourceMng::m_pInstance->CreateRscTexture(strFullPath.c_str());
		assert(pRscTexture!=NULL);
		if(pRscTexture)
			SetMapNormal(pRscTexture);
	}

	type = 0;
	stream.read((char*)&type,sizeof(type));
	assert(type<MAX);
	fileName.clear();	
	ReadString(stream,fileName);	
	if (fileName.length()!=0)
	{
		std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
		std::string strFullPath = strDataPath;
		strFullPath += fileName;
		cRscTexture* pRscTexture= cResourceMng::m_pInstance->CreateRscTexture(strFullPath.c_str());
		assert(pRscTexture!=NULL);
		if(pRscTexture)
			SetMapLight(pRscTexture);
	}

	if (ver >= MATERIAL_LASTEST)
	{
		type = 0;
		stream.read((char*)&type,sizeof(type));
		assert(type<MAX);
		fileName.clear();	
		ReadString(stream,fileName);	
		if (fileName.length()!=0)
		{
			std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
			std::string strFullPath = strDataPath;
			strFullPath += fileName;
			cRscTexture* pRscTexture= cResourceMng::m_pInstance->CreateRscTexture(strFullPath.c_str());
			assert(pRscTexture!=NULL);
			if(pRscTexture)
				SetMapOpacity(pRscTexture);
		}
	}

	if (ver >= MATERIAL_LASTEST)
	{
		type = 0;
		stream.read((char*)&type,sizeof(type));
		assert(type<MAX);
		fileName.clear();	
		ReadString(stream,fileName);	
		if (fileName.length()!=0)
		{
			std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
			std::string strFullPath = strDataPath;
			strFullPath += fileName;
			cRscTexture* pRscTexture= cResourceMng::m_pInstance->CreateRscTexture(strFullPath.c_str());
			assert(pRscTexture!=NULL);
			if(pRscTexture)
				SetMapSpecular(pRscTexture);
		}
	}

	if (ver >= MATERIAL_LASTEST)
	{
		bool bAlphaBlend;
		stream.read((char*)&bAlphaBlend,sizeof(AlphaBlend));
		AlphaBlend = bAlphaBlend;	

		bool bAlphaTestEnable;
		stream.read((char*)&bAlphaTestEnable,sizeof(AlphaTestEnable));
		AlphaTestEnable = bAlphaTestEnable;		

		stream.read((char*)&AlphaTestRef,sizeof(AlphaTestRef));
	}
	

}

void Material::SerializeOut( std::ofstream& stream )
{
	unsigned short ver = MATERIAL_LASTEST;
	stream.write((char*)&ver,sizeof(ver));
	
	unsigned char type = 0;
	std::string fileName;
	type = DIFFUSE;
	stream.write((char*)&type,sizeof(type));
	fileName.clear();
	if (m_pMapDiffuse)
	{
		StringUtil::SplitPath(std::string(m_pMapDiffuse->GetFilePath()),NULL,NULL,&fileName,&fileName);
	}
	WriteString(stream,fileName);

	type = NORMAL;
	stream.write((char*)&type,sizeof(type));
	fileName.clear();
	if (m_pMapNormal)
	{
		StringUtil::SplitPath(std::string(m_pMapNormal->GetFilePath()),NULL,NULL,&fileName,&fileName);
	}
	WriteString(stream,fileName);

	type = LIGHT;
	stream.write((char*)&type,sizeof(type));
	fileName.clear();
	if (m_pMapLight)
	{
		StringUtil::SplitPath(std::string(m_pMapLight->GetFilePath()),NULL,NULL,&fileName,&fileName);
	}
	WriteString(stream,fileName);	

	type = OPACITY;
	stream.write((char*)&type,sizeof(type));
	fileName.clear();
	if (m_pMapOpacity)
	{
		StringUtil::SplitPath(std::string(m_pMapOpacity->GetFilePath()),NULL,NULL,&fileName,&fileName);
	}
	WriteString(stream,fileName);	

	type = SPECULAR;
	stream.write((char*)&type,sizeof(type));
	fileName.clear();
	if (m_pMapSpecular)
	{
		StringUtil::SplitPath(std::string(m_pMapSpecular->GetFilePath()),NULL,NULL,&fileName,&fileName);
	}
	WriteString(stream,fileName);	

	stream.write((char*)&AlphaBlend,sizeof(AlphaBlend));
	stream.write((char*)&AlphaTestEnable,sizeof(AlphaTestEnable));		
	stream.write((char*)&AlphaTestRef,sizeof(AlphaTestRef));		
}



BOOL Material::Create()
{
	assert(m_RefCounter>=0);
	if (m_RefCounter > 0 )
		return TRUE;

	return TRUE;
}

void Material::Free()
{
	cResourceMng::m_pInstance->EraseMaterial(GetUniqueKey());
	delete this;
}


}