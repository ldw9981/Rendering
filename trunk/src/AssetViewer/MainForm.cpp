#include "StdAfx.h"
#include "MainForm.h"
#include "State.h"

System::Void AssetViewer::MainForm::OnShown( System::Object^ sender, System::EventArgs^ e )
{
	sceneTreeForm->MdiParent = this;
	sceneTreeForm->treeView1->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &MainForm::sceneTreeForm_treeView1_AfterSelect);
	sceneTreeForm->treeView1->NodeMouseClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &MainForm::sceneTreeForm_treeView1_NodeMouseClick);

	int CaptionHeight = sceneTreeForm->Height - sceneTreeForm->ClientSize.Height;
	sceneTreeForm->Size = System::Drawing::Size(400, this->ClientSize.Height - CaptionHeight);
	sceneTreeForm->Show();
	sceneTreeForm->Location = System::Drawing::Point(0, 0);

	animationForm->MdiParent = this;
	animationForm->Show();
	animationForm->Location = System::Drawing::Point(sceneTreeForm->Size.Width+viewForm->Size.Width, 0);
	animationForm->listAnimation->SelectedIndexChanged += gcnew System::EventHandler(sceneTreeForm, &SceneTreeForm::listAnimation_SelectedIndexChanged);
	animationForm->checkBox_showPartialWeight->CheckedChanged += gcnew System::EventHandler(sceneTreeForm, &SceneTreeForm::checkBox_showPartialWeight_CheckedChanged);

	viewForm->MdiParent = this;
	viewForm->Shown += gcnew System::EventHandler(this, &MainForm::viewForm_OnShown);
	viewForm->Show();
	viewForm->Location = System::Drawing::Point(sceneTreeForm->Size.Width, 0); 
	OutputDebugString(L"MainForm::OnShown");
}

System::Void AssetViewer::MainForm::shadowOnOffToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
{
	m_pState->m_graphicWorld.SetEnableShadow(!m_pState->m_graphicWorld.GetEnableShadow());
}

System::Void AssetViewer::MainForm::skeletonShowHideToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
{
	if (m_pState==NULL)
		return;

	m_bShowSkeleton = !m_bShowSkeleton;
	m_pState->GetEntity()->SetShowSkeleton(m_bShowSkeleton);
}

System::Void AssetViewer::MainForm::MainForm_Deactivate( System::Object^ sender, System::EventArgs^ e )
{
	if(viewForm->m_pFramework)
	{
		viewForm->m_pFramework->GetInput()->UnAcquire();
	}
	OutputDebugString(L"MainForm_Deactivate");
}

System::Void AssetViewer::MainForm::MainForm_Activated( System::Object^ sender, System::EventArgs^ e )
{
	if(viewForm->m_pFramework)
	{
		viewForm->Activate();				
		viewForm->m_pFramework->GetInput()->Acquire();
	}
	OutputDebugString(L"MainForm_Activated");
}

void AssetViewer::MainForm::Pop_scenePropertyForm()
{
	//assert(scenePropertyForm==nullptr);
	if (scenePropertyForm!=nullptr)
		return;

	scenePropertyForm = gcnew ScenePropertyForm;
	scenePropertyForm->Owner = this;
	scenePropertyForm->Closed += gcnew System::EventHandler(this, &MainForm::scenePropertyForm_Closed);
	scenePropertyForm->Show();
}

void AssetViewer::MainForm::Pop_materialPropertyForm()
{
	//assert(materialPropertyForm==nullptr);
	if (materialPropertyForm!=nullptr)
		return;

	materialPropertyForm = gcnew MaterialPropertyForm;
	materialPropertyForm->Owner = this;
	materialPropertyForm->Closed += gcnew System::EventHandler(this, &MainForm::materialPropertyForm_Closed);
	materialPropertyForm->Show();
}

System::Void AssetViewer::MainForm::contextMenuStrip1_ItemClicked( System::Object^ sender, System::Windows::Forms::ToolStripItemClickedEventArgs^ e )
{
	int index = e->ClickedItem->Owner->Items->IndexOf(e->ClickedItem);				
	switch (index)
	{
	case 0:
		Pop_scenePropertyForm();
		if ( scenePropertyForm != nullptr )
			scenePropertyForm->Update(m_pState,m_pNode);

		break;
	case 1:
		Pop_materialPropertyForm();
		if ( materialPropertyForm!= nullptr )
			materialPropertyForm->Update(m_pState,m_pNode);

		break;
	}
}

System::Void AssetViewer::MainForm::scenePropertyForm_Closed( System::Object^ sender, System::EventArgs^ e )
{
	//assert(scenePropertyForm!=nullptr);
	delete scenePropertyForm;
	scenePropertyForm = nullptr;
}

System::Void AssetViewer::MainForm::materialPropertyForm_Closed( System::Object^ sender, System::EventArgs^ e )
{
	//ASSERT(materialPropertyForm!=nullptr);
	delete materialPropertyForm;
	materialPropertyForm = nullptr;
}

System::Void AssetViewer::MainForm::aSEToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
{
	OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

	openFileDialog1->InitialDirectory = ".\\";
	openFileDialog1->Filter = "ase files (*.ase)|*.ase";
	openFileDialog1->FilterIndex = 1;
	openFileDialog1->RestoreDirectory = true;

	if ( openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK )
	{
		State* pState = (State*)Sophia::cD3DFramework::m_pInstance->GetView();

		std::string path = msclr::interop::marshal_as< std::string >( openFileDialog1->FileName); 
		pState->OpenASE(path.c_str());
		pState->GetEntity()->SetShowSkeleton(m_bShowSkeleton);
		sceneTreeForm->Clear();
		sceneTreeForm->Update(pState);

		animationForm->Clear();
		animationForm->Update(pState,NULL);
		m_pNode = NULL;


		if ( scenePropertyForm!= nullptr )
			scenePropertyForm->Update(pState,NULL);

		if ( materialPropertyForm!= nullptr )
			materialPropertyForm->Update(pState,NULL);
	}
	delete openFileDialog1;
}

System::Void AssetViewer::MainForm::assetToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
{
	OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

	openFileDialog1->InitialDirectory = ".\\";
	openFileDialog1->Filter = "scene files (*.scene)|*.scene";
	openFileDialog1->FilterIndex = 1;
	openFileDialog1->RestoreDirectory = true;

	if ( openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK )
	{
		State* pState = (State*)Sophia::cD3DFramework::m_pInstance->GetView();

		std::string path = msclr::interop::marshal_as< std::string >( openFileDialog1->FileName); 
		pState->OpenAsset(path.c_str());
		pState->GetEntity()->SetShowSkeleton(m_bShowSkeleton);

		sceneTreeForm->Clear();
		sceneTreeForm->Update(pState);

		animationForm->Clear();
		animationForm->Update(pState,m_pNode);
		m_pNode = NULL;

		if ( scenePropertyForm!= nullptr )
			scenePropertyForm->Update(pState,m_pNode);

		if ( materialPropertyForm!= nullptr )
			materialPropertyForm->Update(pState,m_pNode);

	}
	delete openFileDialog1;
}

System::Void AssetViewer::MainForm::allToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
{
	m_pState->SaveAsset();

	animationForm->Update(m_pState,m_pNode);
}

System::Void AssetViewer::MainForm::sceneTreeForm_treeView1_AfterSelect( System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e )
{
	std::string name = msclr::interop::marshal_as< std::string >(e->Node->Text);
	m_pNode = m_pState->GetEntity()->FindNode(name);
	animationForm->Update(m_pState,m_pNode);

	if ( scenePropertyForm != nullptr )
		scenePropertyForm->Update(m_pState,m_pNode);

	if ( materialPropertyForm!= nullptr )
		materialPropertyForm->Update(m_pState,m_pNode);
}

System::Void AssetViewer::MainForm::sceneTreeForm_treeView1_NodeMouseClick( System::Object^ sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^ e )
{
	if (e->Button != System::Windows::Forms::MouseButtons::Right)
		return;

	if (m_pNode == NULL)
		return;

	std::string name = msclr::interop::marshal_as< std::string >(e->Node->Text);
	if( m_pNode->GetNodeName() != name )
		return;

	System::Drawing::Point pt = sceneTreeForm->PointToScreen(System::Drawing::Point(0,0));
	pt.X += e->Location.X;
	pt.Y += e->Location.Y;

	contextMenuStrip1->Show(pt);
}

System::Void AssetViewer::MainForm::materialPropertyForm_numericUpDown1_ValueChanged( System::Object^ sender, System::EventArgs^ e )
{
	materialPropertyForm->Update(m_pState,m_pNode);
}

System::Void AssetViewer::MainForm::viewForm_OnShown( System::Object^ sender, System::EventArgs^ e )
{
	m_pState = (State*)Sophia::cD3DFramework::m_pInstance->GetView();
}

System::Void AssetViewer::MainForm::materialPropertyForm_comboBox1_SelectedIndexChanged( System::Object^ sender, System::EventArgs^ e )
{
	materialPropertyForm->Update(m_pState,m_pNode);
}

System::Void AssetViewer::MainForm::openToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
{

}

System::Void AssetViewer::MainForm::MainForm_Load( System::Object^ sender, System::EventArgs^ e )
{

}

System::Void AssetViewer::MainForm::saveToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
{

}

System::Void AssetViewer::MainForm::cameraSettingsForm_Closed( System::Object^ sender, System::EventArgs^ e )
{

}

System::Void AssetViewer::MainForm::Pop_cameraSettingsForm()
{

}
