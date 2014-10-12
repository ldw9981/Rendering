#pragma once

using namespace System::ComponentModel;

namespace Sophia
{
	class cSceneNode;
}


ref class SceneProperty 
{
public:
	SceneProperty(void);
	~SceneProperty();
public:	
    [Category("Identification")]
	property System::String^ name;

	[Category("Identification")]
	property System::String^ parent;	

	[Category("Identification")]
	property System::String^ type;	

	[Category("Identification")]
	property bool show;	

	[Category("Identification")]
	property bool animationEnable;	

	[Category("Identification")]
	property bool instancingEnable;	

	[Category("Material")]
	property int materialRefIndex;

	[Category("Material")]
	property int materialSubIndex;

	[Category("Transform")]
	property float scale0_x;
	[Category("Transform")]
	property float scale1_y;
	[Category("Transform")]
	property float scale2_z;
	[Category("Transform")]
	property float scale3_w;

	[Category("Transform")]
	property float rotation0_x;
	[Category("Transform")]
	property float rotation1_y;
	[Category("Transform")]
	property float rotation2_z;
	[Category("Transform")]
	property float rotation3_w;

	[Category("Transform")]
	property float translation0_x;
	[Category("Transform")]
	property float translation1_y;
	[Category("Transform")]
	property float translation2_z;
	[Category("Transform")]
	property float translation3_w;





public:
	void Read(Sophia::cSceneNode* pNode);
	void Write(Sophia::cSceneNode* pNode);
};

