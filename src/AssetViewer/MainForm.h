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
	/// MainForm�� ���� ����Դϴ�.
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
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
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
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
		/// �ʼ� �����̳� �����Դϴ�.
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
	private: System::Windows::Forms::ToolStripMenuItem^  skeletonToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem_Skeleton_ShowHide;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  scenePropertyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  materialPropertyToolStripMenuItem;



	private: System::Windows::Forms::ToolStripMenuItem^  materialToolStripMenuItem;

		 
#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
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
			this->skeletonToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem_Skeleton_ShowHide = (gcnew System::Windows::Forms::ToolStripMenuItem());
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
			this->optionToolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->skeletonToolStripMenuItem});
			this->optionToolStripMenuItem1->Name = L"optionToolStripMenuItem1";
			this->optionToolStripMenuItem1->Size = System::Drawing::Size(56, 20);
			this->optionToolStripMenuItem1->Text = L"Option";
			// 
			// skeletonToolStripMenuItem
			// 
			this->skeletonToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripMenuItem_Skeleton_ShowHide});
			this->skeletonToolStripMenuItem->Name = L"skeletonToolStripMenuItem";
			this->skeletonToolStripMenuItem->Size = System::Drawing::Size(120, 22);
			this->skeletonToolStripMenuItem->Text = L"Skeleton";
			// 
			// toolStripMenuItem_Skeleton_ShowHide
			// 
			this->toolStripMenuItem_Skeleton_ShowHide->Name = L"toolStripMenuItem_Skeleton_ShowHide";
			this->toolStripMenuItem_Skeleton_ShowHide->Size = System::Drawing::Size(134, 22);
			this->toolStripMenuItem_Skeleton_ShowHide->Text = L"Show/Hide";
			this->toolStripMenuItem_Skeleton_ShowHide->Click += gcnew System::EventHandler(this, &MainForm::toolStripMenuItem_Skeleton_ShowHide_Click);
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
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->Shown += gcnew System::EventHandler(this, &MainForm::OnShown);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->contextMenuStrip1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void OnShown(System::Object^  sender, System::EventArgs^  e) {

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
				 pState->GetEntity()->SetShowSkeleton(m_bShowSkeleton);
				 sceneTreeForm->Update(pState);
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

				 sceneTreeForm->Update(pState);
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
			 sceneTreeForm->Update(m_pState);
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
private: System::Void toolStripMenuItem_Skeleton_ShowHide_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (m_pState==NULL)
				 return;

			 m_bShowSkeleton = !m_bShowSkeleton;
			 m_pState->GetEntity()->SetShowSkeleton(m_bShowSkeleton);
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
			 
};
}