#include "StdAfx.h"
#include "SceneProperty.h"
#include "Scene/SceneNode.h"
#include "Scene/MeshNode.h"

SceneProperty::SceneProperty(void)
{
	name = gcnew System::String("");
	parent = gcnew System::String("");
	type = gcnew System::String("");
}

SceneProperty::~SceneProperty()
{
	delete name;
	delete parent;
	delete type;
}

void SceneProperty::Read( Sophia::cSceneNode* pNode )
{
	delete name;
	name = gcnew System::String(pNode->GetNodeName().c_str());
	delete parent;
	parent = gcnew System::String(pNode->GetParentName().c_str());
	show = pNode->GetShow();
	
	animationEnable = pNode->GetIsActiveAnimation();

	D3DXVECTOR3 outScale,outTranslation;
	D3DXQUATERNION outRotation;
	D3DXMatrixDecompose(&outScale,&outRotation,&outTranslation,&pNode->GetLocalTM());

	scale0_x = outScale.x;
	scale1_y = outScale.y;
	scale2_z = outScale.z;
	scale3_w = 0.0f;

	rotation0_x = outRotation.x;
	rotation1_y = outRotation.y;
	rotation2_z = outRotation.z;
	rotation3_w = outRotation.w;

	translation0_x = outTranslation.x;
	translation1_y = outTranslation.y;
	translation2_z = outTranslation.z;
	translation3_w = 0.0f;

	Sophia::cMeshNode* pMesh = dynamic_cast<Sophia::cMeshNode*>(pNode);
	if (pMesh!=NULL)
	{
		materialRefIndex = pMesh->GetMaterialRefIndex();
		materialSubIndex = pMesh->GetMaterialSubIndex();
		instancingEnable = pMesh->GetInstancingEnable();
	}
	else
	{
		materialRefIndex = -1;
		materialSubIndex = -1;
		instancingEnable = false;
	}
}

void SceneProperty::Write( Sophia::cSceneNode* pNode )
{
	pNode->SetIsActiveAnimation(animationEnable);
	Sophia::cMeshNode* pMesh = dynamic_cast<Sophia::cMeshNode*>(pNode);
	if (pMesh!=NULL)
	{
		pMesh->ChangeInstancingEnable(instancingEnable);		
	}
}




