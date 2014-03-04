#include "StdAfx.h"
#include "ScenePropertyForm.h"
#include "State.h"
#include "Scene/SceneNode.h"

void AssetViewer::ScenePropertyForm::Update( State* pState,Sophia::cSceneNode* pNode )
{	
	m_pNode = pNode;
	if (pNode!=NULL)
	{
		propertyData->Read(pNode);
		propertyGrid1->SelectedObject = propertyData;
	}
	else
	{
		propertyGrid1->SelectedObject = nullptr;
	}
	
}
