#include "StdAfx.h"
#include "MaterialProperty.h"
#include "Graphics/MaterialEx.h"
#include "Graphics/RscTexture.h"
#include "Foundation/StringUtil.h"

MaterialProperty::MaterialProperty(void)
{
	diffuseMap = gcnew System::String("");
	normalMap = gcnew System::String("");
	lightMap = gcnew System::String("");
	opacityMap = gcnew System::String("");
	alphaBlend = false;
	alphaTestEnable = false;
}


MaterialProperty::~MaterialProperty(void)
{
	delete diffuseMap;
	delete normalMap;
	delete lightMap;
	delete opacityMap;
}

void MaterialProperty::Read( Sophia::Material* pMaterial )
{
	delete diffuseMap;
	delete normalMap;
	delete lightMap;
	delete opacityMap;

	std::string fullpath;
	if (pMaterial->GetMapDiffuse())
	{		
		fullpath = pMaterial->GetMapDiffuse()->GetFilePath();
		std::string name;
		Sophia::StringUtil::SplitPath(fullpath,NULL,NULL,&name,&name);
		diffuseMap = gcnew System::String(name.c_str());
	}
	else
	{
		diffuseMap = gcnew System::String("");
	}

	if (pMaterial->GetMapNormal())
	{
		
		fullpath = pMaterial->GetMapNormal()->GetFilePath();
		std::string name;
		Sophia::StringUtil::SplitPath(fullpath,NULL,NULL,&name,&name);
		normalMap = gcnew System::String(name.c_str());
	}
	else
	{
		normalMap = gcnew System::String("");
	}

	if (pMaterial->GetMapLight())
	{
		
		fullpath = pMaterial->GetMapLight()->GetFilePath();
		std::string name;
		Sophia::StringUtil::SplitPath(fullpath,NULL,NULL,&name,&name);
		lightMap = gcnew System::String(name.c_str());
	}
	else
	{
		lightMap = gcnew System::String("");
	}

	if (pMaterial->GetMapOpacity())
	{

		fullpath = pMaterial->GetMapOpacity()->GetFilePath();
		std::string name;
		Sophia::StringUtil::SplitPath(fullpath,NULL,NULL,&name,&name);
		opacityMap = gcnew System::String(name.c_str());
	}
	else
	{
		opacityMap = gcnew System::String("");
	}

	alphaBlend = pMaterial->AlphaBlend;
	alphaTestEnable = pMaterial->AlphaTestEnable;
	alphaTestRef = pMaterial->AlphaTestRef;
}

void MaterialProperty::Write( Sophia::Material* pMaterial )
{
	pMaterial->AlphaBlend = alphaBlend;
	pMaterial->AlphaTestEnable = alphaTestEnable;

	alphaTestRef = min(alphaTestRef,255);
	pMaterial->AlphaTestRef = (unsigned char)alphaTestRef;
}
