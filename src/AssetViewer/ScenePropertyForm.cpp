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

System::Void AssetViewer::ScenePropertyForm::propertyGrid1_PropertyValueChanged( System::Object^ s, System::Windows::Forms::PropertyValueChangedEventArgs^ e )
{
	// ��� �̸��� �ٲ������
	// �θ� ��� �̸��� �ٲ������
	// ������ �����Ͱ� �ٲ������

	if (m_pNode==NULL)
		return;

	propertyData->Write(m_pNode);
	m_pNode->GetRootNode()->ResetRenderContainer();
	
}
