#pragma once
#include "ViewForm.h"
#include "SceneTreeForm.h"
#include "NodePropertyForm.h"
#include "AnimationForm.h"
#include "Form1.h"

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
			nodePropertyForm = gcnew NodePropertyForm;
			animationForm = gcnew AnimationForm;
			testForm = gcnew Form1;
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
			delete nodePropertyForm;
			delete sceneTreeForm;
			delete viewForm;
		}


	protected: 
		static State* m_pState;
	private:
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>
		System::ComponentModel::Container ^components;
		ViewForm^ viewForm;
		SceneTreeForm^ sceneTreeForm;
		AnimationForm^ animationForm;
		NodePropertyForm^		nodePropertyForm;
		Form1^		testForm;

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
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
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
				 viewForm->Show();

				 sceneTreeForm->MdiParent = this;
				 sceneTreeForm->Show();

				 nodePropertyForm->MdiParent = this;
				 nodePropertyForm->Show();

				 animationForm->MdiParent = this;
				 animationForm->Show();

				 testForm->MdiParent = this;
				 testForm->Show();
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
				 animationForm->Update(pState);
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
				 animationForm->Update(pState);
			 }
			 delete openFileDialog1;
		 }
private: System::Void allToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 State* pState = (State*)Sophia::cD3DFramework::m_pInstance->GetView();
			 pState->SaveAsset();
		 }
};
}
