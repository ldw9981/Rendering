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

	viewForm->MdiParent = this;
	viewForm->Shown += gcnew System::EventHandler(this, &MainForm::viewForm_OnShown);
	viewForm->Show();
	viewForm->Location = System::Drawing::Point(sceneTreeForm->Size.Width, 0); 


	animationForm->MdiParent = this;
	animationForm->Show();
	animationForm->Location = System::Drawing::Point(sceneTreeForm->Size.Width+viewForm->Size.Width, 0);
	animationForm->listAnimation->SelectedIndexChanged += gcnew System::EventHandler(sceneTreeForm, &SceneTreeForm::listAnimation_SelectedIndexChanged);
	animationForm->checkBox_showPartialWeight->CheckedChanged += gcnew System::EventHandler(sceneTreeForm, &SceneTreeForm::checkBox_showPartialWeight_CheckedChanged);
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
