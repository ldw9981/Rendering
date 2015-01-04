#include "StdAfx.h"
#include "ScenePropertyForm.h"
#include "State.h"
#include "Graphics/SceneNode.h"

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

System::Void AssetViewer::ScenePropertyForm::propertyGrid1_PropertyValueChanged( System::Object^ s, System::Windows::Forms::PropertyValueChangedEventArgs^ e )
{
	// 노드 이름이 바뀌었을때
	// 부모 노드 이름이 바뀌었을때
	// 나머지 데이터가 바뀌었을때

	if (m_pNode==NULL)
		return;

	propertyData->Write(m_pNode);
	m_pNode->GetRootNode()->ResetRenderContainer();
	
}
