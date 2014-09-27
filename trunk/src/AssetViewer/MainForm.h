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
			animationForm = gcnew AnimationForm;
			
			scenePropertyForm = nullptr;
			materialPropertyForm = nullptr;
			m_pNode = NULL;
			m_bShowSkeleton = false;
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
			
			delete animationForm;			
			delete sceneTreeForm;
			delete viewForm;
		}


	protected: 
		State* m_pState;
		Sophia::cSceneNode* m_pNode;
	private: System::ComponentModel::IContainer^  components;
	protected: 
	private:
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>

		ViewForm^ viewForm;
		SceneTreeForm^ sceneTreeForm;
		AnimationForm^ animationForm;
		ScenePropertyForm^		scenePropertyForm;
		MaterialPropertyForm^	materialPropertyForm;
		bool					m_bShowSkeleton;
		

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
	private: System::Windows::Forms::ToolStripMenuItem^  optionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  optionToolStripMenuItem1;


	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  scenePropertyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  materialPropertyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  viewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  shadowOnOffToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  skeletonShowHideToolStripMenuItem;



	private: System::Windows::Forms::ToolStripMenuItem^  materialToolStripMenuItem;

		 
#pragma region Windows Form Designer generated code
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
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
			this->optionToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->shadowOnOffToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->skeletonShowHideToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->optionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->scenePropertyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->materialPropertyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			this->contextMenuStrip1->SuspendLayout();
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
			this->openToolStripMenuItem->Size = System::Drawing::Size(103, 22);
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
			this->saveToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveToolStripMenuItem_Click);
			// 
			// allToolStripMenuItem
			// 
			this->allToolStripMenuItem->Name = L"allToolStripMenuItem";
			this->allToolStripMenuItem->Size = System::Drawing::Size(147, 22);
			this->allToolStripMenuItem->Text = L"All";
			this->allToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::allToolStripMenuItem_Click);
			// 
			// sceneToolStripMenuItem
			// 
			this->sceneToolStripMenuItem->Name = L"sceneToolStripMenuItem";
			this->sceneToolStripMenuItem->Size = System::Drawing::Size(147, 22);
			this->sceneToolStripMenuItem->Text = L"Scene";
			// 
			// animationToolStripMenuItem
			// 
			this->animationToolStripMenuItem->Name = L"animationToolStripMenuItem";
			this->animationToolStripMenuItem->Size = System::Drawing::Size(147, 22);
			this->animationToolStripMenuItem->Text = L"AnimationSet";
			// 
			// materialToolStripMenuItem
			// 
			this->materialToolStripMenuItem->Name = L"materialToolStripMenuItem";
			this->materialToolStripMenuItem->Size = System::Drawing::Size(147, 22);
			this->materialToolStripMenuItem->Text = L"Material";
			// 
			// closeToolStripMenuItem
			// 
			this->closeToolStripMenuItem->Name = L"closeToolStripMenuItem";
			this->closeToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->closeToolStripMenuItem->Text = L"Close";
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
				this->optionToolStripMenuItem1});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1280, 24);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// optionToolStripMenuItem1
			// 
			this->optionToolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->viewToolStripMenuItem});
			this->optionToolStripMenuItem1->Name = L"optionToolStripMenuItem1";
			this->optionToolStripMenuItem1->Size = System::Drawing::Size(56, 20);
			this->optionToolStripMenuItem1->Text = L"Option";
			// 
			// viewToolStripMenuItem
			// 
			this->viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->shadowOnOffToolStripMenuItem, 
				this->skeletonShowHideToolStripMenuItem});
			this->viewToolStripMenuItem->Name = L"viewToolStripMenuItem";
			this->viewToolStripMenuItem->Size = System::Drawing::Size(100, 22);
			this->viewToolStripMenuItem->Text = L"View";
			// 
			// shadowOnOffToolStripMenuItem
			// 
			this->shadowOnOffToolStripMenuItem->Name = L"shadowOnOffToolStripMenuItem";
			this->shadowOnOffToolStripMenuItem->Size = System::Drawing::Size(184, 22);
			this->shadowOnOffToolStripMenuItem->Text = L"Shadow On/Off";
			this->shadowOnOffToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::shadowOnOffToolStripMenuItem_Click);
			// 
			// skeletonShowHideToolStripMenuItem
			// 
			this->skeletonShowHideToolStripMenuItem->Name = L"skeletonShowHideToolStripMenuItem";
			this->skeletonShowHideToolStripMenuItem->Size = System::Drawing::Size(184, 22);
			this->skeletonShowHideToolStripMenuItem->Text = L"Skeleton Show/Hide";
			this->skeletonShowHideToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::skeletonShowHideToolStripMenuItem_Click);
			// 
			// optionToolStripMenuItem
			// 
			this->optionToolStripMenuItem->Name = L"optionToolStripMenuItem";
			this->optionToolStripMenuItem->Size = System::Drawing::Size(32, 19);
			this->optionToolStripMenuItem->Text = L"Option";
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->scenePropertyToolStripMenuItem, 
				this->materialPropertyToolStripMenuItem});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(167, 48);
			this->contextMenuStrip1->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &MainForm::contextMenuStrip1_ItemClicked);
			// 
			// scenePropertyToolStripMenuItem
			// 
			this->scenePropertyToolStripMenuItem->Name = L"scenePropertyToolStripMenuItem";
			this->scenePropertyToolStripMenuItem->Size = System::Drawing::Size(166, 22);
			this->scenePropertyToolStripMenuItem->Tag = L"";
			this->scenePropertyToolStripMenuItem->Text = L"Scene Property";
			// 
			// materialPropertyToolStripMenuItem
			// 
			this->materialPropertyToolStripMenuItem->Name = L"materialPropertyToolStripMenuItem";
			this->materialPropertyToolStripMenuItem->Size = System::Drawing::Size(166, 22);
			this->materialPropertyToolStripMenuItem->Tag = L"";
			this->materialPropertyToolStripMenuItem->Text = L"Material Property";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1280, 720);
			this->Controls->Add(this->menuStrip1);
			this->IsMdiContainer = true;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->Activated += gcnew System::EventHandler(this, &MainForm::MainForm_Activated);
			this->Deactivate += gcnew System::EventHandler(this, &MainForm::MainForm_Deactivate);
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->Shown += gcnew System::EventHandler(this, &MainForm::OnShown);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->contextMenuStrip1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: 
		System::Void OnShown(System::Object^  sender, System::EventArgs^  e);


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
private: System::Void allToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			
			 m_pState->SaveAsset();
		
			 animationForm->Update(m_pState,m_pNode);
		 }
	public: System::Void sceneTreeForm_treeView1_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
				
				std::string name = msclr::interop::marshal_as< std::string >(e->Node->Text);
				m_pNode = m_pState->GetEntity()->FindNode(name);
				animationForm->Update(m_pState,m_pNode);

				if ( scenePropertyForm != nullptr )
					scenePropertyForm->Update(m_pState,m_pNode);

				if ( materialPropertyForm!= nullptr )
					materialPropertyForm->Update(m_pState,m_pNode);
			}
	public: System::Void sceneTreeForm_treeView1_NodeMouseClick(System::Object^  sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^  e) {
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
	private: System::Void materialPropertyForm_numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				materialPropertyForm->Update(m_pState,m_pNode);
		 }
	private: System::Void viewForm_OnShown(System::Object^  sender, System::EventArgs^  e) {
				  m_pState = (State*)Sophia::cD3DFramework::m_pInstance->GetView();
			 }
	private: System::Void materialPropertyForm_comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 materialPropertyForm->Update(m_pState,m_pNode);
		 }

	private: System::Void scenePropertyForm_Closed(System::Object^  sender, System::EventArgs^  e) 
		{
			//assert(scenePropertyForm!=nullptr);
			delete scenePropertyForm;
			scenePropertyForm = nullptr;			
		}
	private: System::Void materialPropertyForm_Closed(System::Object^  sender, System::EventArgs^  e) 
		{
			//ASSERT(materialPropertyForm!=nullptr);
			delete materialPropertyForm;
			materialPropertyForm = nullptr;	
		}
	public: void Pop_scenePropertyForm()
		{
			//assert(scenePropertyForm==nullptr);
			if (scenePropertyForm!=nullptr)
				return;

			scenePropertyForm = gcnew ScenePropertyForm;
			scenePropertyForm->Owner = this;
			scenePropertyForm->Closed += gcnew System::EventHandler(this, &MainForm::scenePropertyForm_Closed);
			scenePropertyForm->Show();		

		}
	public: void Pop_materialPropertyForm()
		{
			//assert(materialPropertyForm==nullptr);
			if (materialPropertyForm!=nullptr)
				return;

			materialPropertyForm = gcnew MaterialPropertyForm;
			materialPropertyForm->Owner = this;
			materialPropertyForm->Closed += gcnew System::EventHandler(this, &MainForm::materialPropertyForm_Closed);
			materialPropertyForm->Show();
		}
	public: System::Void contextMenuStrip1_ItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) {
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
	private: System::Void shadowOnOffToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void skeletonShowHideToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void MainForm_Deactivate(System::Object^  sender, System::EventArgs^  e);
private: System::Void MainForm_Activated(System::Object^  sender, System::EventArgs^  e);
};
}
