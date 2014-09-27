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
