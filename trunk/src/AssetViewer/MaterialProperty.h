#pragma once

using namespace System::ComponentModel;

namespace Sophia
{
	class Material;
}

ref class MaterialProperty
{
public:
	MaterialProperty(void);
	virtual ~MaterialProperty(void);

	[Category("Material")]
	property System::String^ diffuseMap;

	[Category("Material")]
	property System::String^ normalMap;	

	[Category("Material")]
	property System::String^ lightMap;	

	[Category("Material")]
	property System::String^ opacityMap;	

	[Category("Material")]
	property System::String^ specularMap;	

	[Category("Material")]
	property System::Boolean alphaBlend;	

	[Category("Material")]
	property System::Boolean alphaTestEnable;	

	[Category("Material")]
	property System::Int16 alphaTestRef;

public:
	void Read(Sophia::Material* pMaterial);
	void Write(Sophia::Material* pMaterial);
};

