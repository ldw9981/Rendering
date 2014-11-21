#include "StdAfx.h"
#include "MaterialPropertyForm.h"
#include "Scene/MeshNode.h"
#include "Graphics/MaterialEx.h"
#include "Graphics/Entity.h"
#include "MaterialProperty.h"

void AssetViewer::MaterialPropertyForm::Update( State* pState,Sophia::cSceneNode* pNode )
{	
	Sophia::cMeshNode* pMesh = dynamic_cast<Sophia::cMeshNode*>(pNode);
	bool bChangeNode=false;
	if (m_pNode != pNode)
	{	
		bChangeNode=true;
	}
	m_pNode = pNode;	

	if (pMesh)
	{
		this->textBoxTotal->Text = Convert::ToString(pMesh->GetRootNode()->m_pEntityMaterial->GetCount());
		this->textBoxRefIndex->Text = Convert::ToString(pMesh->GetMaterialRefIndex()); 
		this->textBoxSubIndex->Text = Convert::ToString(pMesh->GetMaterialSubIndex()); 
	
		if (bChangeNode)
		{			
			propertyData->Read(pMesh->GetMaterial());
			propertyGrid1->SelectedObject = propertyData;			
		}
	}
	else
	{
		this->textBoxTotal->Text = "";
		this->textBoxRefIndex->Text = "";
		this->textBoxSubIndex->Text = "";		
		propertyGrid1->SelectedObject = nullptr;
	}

	
}



System::Void AssetViewer::MaterialPropertyForm::propertyGrid1_PropertyValueChanged( System::Object^ s, System::Windows::Forms::PropertyValueChangedEventArgs^ e )
{
	if (m_pNode==NULL)
		return;

	Sophia::cMeshNode* pMesh = dynamic_cast<Sophia::cMeshNode*>(m_pNode);
	if (pMesh==NULL)
		return;

	Sophia::Material* pMaterial = pMesh->GetMaterial();

	if (pMaterial!=NULL)
	{
		propertyData->Write(pMaterial);

		pMesh->GetRootNode()->ResetRenderContainer();
	}
}
