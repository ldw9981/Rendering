#include "StdAfx.h"
#include "MaterialPropertyForm.h"
#include "Scene/MeshNode.h"
#include "Graphics/MaterialEx.h"
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
		Sophia::MeshMaterials* pSceneMaterial = pMesh->GetMeshMaterials();
		if (bChangeNode)
		{			
			this->comboBox1->Items->Clear();
			for (size_t i=0;i<pSceneMaterial->m_container.size();i++)
			{
				this->comboBox1->Items->Add(Convert::ToString(i));			
			}		
			this->comboBox1->SelectedIndex = 0;			
		}
	}
	else
	{
		this->comboBox1->Items->Clear();
		propertyGrid1->SelectedObject = nullptr;
	}

	
}

System::Void AssetViewer::MaterialPropertyForm::comboBox1_SelectedIndexChanged( System::Object^ sender, System::EventArgs^ e )
{
	Sophia::cMeshNode* pMesh = dynamic_cast<Sophia::cMeshNode*>(m_pNode);
	if (pMesh==NULL)
		return;

	Sophia::MeshMaterials* pSceneMaterial = pMesh->GetMeshMaterials();
	Sophia::Material* pMaterial = NULL;
	int index = this->comboBox1->SelectedIndex;
	if (index != -1)
	{
		pMaterial = &pSceneMaterial->m_container[index];
	}

	if (pMaterial!=NULL)
	{
		propertyData->Read(pMaterial);
		propertyGrid1->SelectedObject = propertyData;
	}
	else
	{
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

	Sophia::MeshMaterials* pSceneMaterial = pMesh->GetMeshMaterials();
	Sophia::Material* pMaterial = NULL;
	int index = this->comboBox1->SelectedIndex;
	if (index != -1)
	{
		pMaterial = &pSceneMaterial->m_container[index];
	}

	if (pMaterial!=NULL)
	{
		propertyData->Write(pMaterial);

		pMesh->GetRootNode()->Build();
	}
}
