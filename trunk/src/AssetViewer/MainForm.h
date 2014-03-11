#pragma once
#include "ViewForm.h"
#include "SceneTreeForm.h"
#include "ScenePropertyForm.h"
#include "AnimationForm.h"
#include "MaterialPropertyForm.h"
#include "Graphics/Entity.h"
#include "Scene/SceneNode.h"

namespace AssetViewer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MainForm에 대한 요약입니다.
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: 생성자 코드를 여기에 추가합니다.
			//
			viewForm = gcnew ViewForm;
	        sceneTreeForm = gcnew SceneTreeForm;
			scenePropertyForm = gcnew ScenePropertyForm;
			animationForm = gcnew AnimationForm;
			materialPropertyForm = gcnew MaterialPropertyForm;
			m_pNode = NULL;
		}

	protected:
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
			
			delete materialPropertyForm;
			delete animationForm;
			delete scenePropertyForm;
			delete sceneTreeForm;
			delete viewForm;
		}


	protected: 
		State* m_pState;
		Sophia::cSceneNode* m_pNode;
	private:
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>
		System::ComponentModel::Container ^components;
		ViewForm^ viewForm;
		SceneTreeForm^ sceneTreeForm;
		AnimationForm^ animationForm;
		ScenePropertyForm^		scenePropertyForm;
		MaterialPropertyForm^	materialPropertyForm;
		

	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  closeToolStripMenuItem;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  aSEToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  assetToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  allToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  sceneToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  animationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  materialToolStripMenuItem;

		 
#pragma region Windows Form Designer generated code
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
		/// </summary>
		void InitializeComponent(void)
		{
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aSEToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->assetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->allToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sceneToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->animationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->materialToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->closeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->openToolStripMenuItem, 
				this->saveToolStripMenuItem, this->closeToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->aSEToolStripMenuItem, 
				this->assetToolStripMenuItem});
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::openToolStripMenuItem_Click);
			// 
			// aSEToolStripMenuItem
			// 
			this->aSEToolStripMenuItem->Name = L"aSEToolStripMenuItem";
			this->aSEToolStripMenuItem->Size = System::Drawing::Size(102, 22);
			this->aSEToolStripMenuItem->Text = L"ASE";
			this->aSEToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::aSEToolStripMenuItem_Click);
			// 
			// assetToolStripMenuItem
			// 
			this->assetToolStripMenuItem->Name = L"assetToolStripMenuItem";
			this->assetToolStripMenuItem->Size = System::Drawing::Size(102, 22);
			this->assetToolStripMenuItem->Text = L"Asset";
			this->assetToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::assetToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->allToolStripMenuItem, 
				this->sceneToolStripMenuItem, this->animationToolStripMenuItem, this->materialToolStripMenuItem});
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveToolStripMenuItem_Click);
			// 
			// allToolStripMenuItem
			// 
			this->allToolStripMenuItem->Name = L"allToolStripMenuItem";
			this->allToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->allToolStripMenuItem->Text = L"All";
			this->allToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::allToolStripMenuItem_Click);
			// 
			// sceneToolStripMenuItem
			// 
			this->sceneToolStripMenuItem->Name = L"sceneToolStripMenuItem";
			this->sceneToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->sceneToolStripMenuItem->Text = L"Scene";
			// 
			// animationToolStripMenuItem
			// 
			this->animationToolStripMenuItem->Name = L"animationToolStripMenuItem";
			this->animationToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->animationToolStripMenuItem->Text = L"AnimationSet";
			// 
			// materialToolStripMenuItem
			// 
			this->materialToolStripMenuItem->Name = L"materialToolStripMenuItem";
			this->materialToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->materialToolStripMenuItem->Text = L"Material";
			// 
			// closeToolStripMenuItem
			// 
			this->closeToolStripMenuItem->Name = L"closeToolStripMenuItem";
			this->closeToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->closeToolStripMenuItem->Text = L"Close";
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fileToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1024, 24);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1024, 768);
			this->Controls->Add(this->menuStrip1);
			this->IsMdiContainer = true;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->Shown += gcnew System::EventHandler(this, &MainForm::OnShown);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void OnShown(System::Object^  sender, System::EventArgs^  e) {
				 viewForm->MdiParent = this;
				 viewForm->Shown += gcnew System::EventHandler(this, &MainForm::viewForm_OnShown);
				 viewForm->Show();

				 sceneTreeForm->MdiParent = this;
				 sceneTreeForm->treeView1->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &MainForm::sceneTreeForm_treeView1_AfterSelect);
				 sceneTreeForm->Show();
				

				 scenePropertyForm->MdiParent = this;
				 scenePropertyForm->Show();
				 

				 animationForm->MdiParent = this;
				 animationForm->Show();

				 materialPropertyForm->MdiParent = this;
				 materialPropertyForm->Show();
			 }


	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			
			 }
	private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void aSEToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
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
				 sceneTreeForm->Update(pState);
				 animationForm->Update(pState,NULL);
				 m_pNode = NULL;
				 scenePropertyForm->Update(pState,NULL);
				 materialPropertyForm->Update(pState,NULL);
			 }
			 delete openFileDialog1;
		 }
private: System::Void assetToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
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
				
				 sceneTreeForm->Update(pState);
				 m_pNode = NULL;
				 scenePropertyForm->Update(pState,m_pNode);
				 materialPropertyForm->Update(pState,m_pNode);
				 animationForm->Update(pState,m_pNode);
			 }
			 delete openFileDialog1;
		 }
private: System::Void allToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			
			 m_pState->SaveAsset();
			 sceneTreeForm->Update(m_pState);
			 animationForm->Update(m_pState,m_pNode);
		 }
	public: System::Void sceneTreeForm_treeView1_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
				
				std::string name = msclr::interop::marshal_as< std::string >(e->Node->Text);
				m_pNode = m_pState->GetEntity()->FindNode(name);
				scenePropertyForm->Update(m_pState,m_pNode);
				materialPropertyForm->Update(m_pState,m_pNode);
				animationForm->Update(m_pState,m_pNode);
			}
	private: System::Void materialPropertyForm_numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				materialPropertyForm->Update(m_pState,m_pNode);
		 }
	private: System::Void viewForm_OnShown(System::Object^  sender, System::EventArgs^  e) {
				  m_pState = (State*)Sophia::cD3DFramework::m_pInstance->GetView();
			 }
	private: System::Void materialPropertyForm_comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 materialPropertyForm->Update(m_pState,m_pNode);
		 }
};
}
