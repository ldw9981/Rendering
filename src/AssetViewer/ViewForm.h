#pragma once
#include "graphics/graphics.h"
#include "Graphics/World.h"
#include "Framework.h"
#include "State.h"
#include "Input/Input.h"

namespace AssetViewer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// ViewForm�� ���� ����Դϴ�.
	/// </summary>
	public ref class ViewForm : public System::Windows::Forms::Form
	{
	public:
		ViewForm(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//
			m_pFramework = NULL;
		}

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~ViewForm()
		{
			if (components)
			{
				delete components;
			}
			m_pFramework->Finalize();
			delete m_pFramework;
			m_pFramework = NULL;
		}
	public:
		Framework*		m_pFramework;
		State*			m_pState;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  showHideShadowToolStripMenuItem;



	private: System::Windows::Forms::ToolStripMenuItem^  resetTransformToolStripMenuItem;
	public: 
	private: System::ComponentModel::IContainer^  components;
	private:
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>

		
#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->showHideShadowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->resetTransformToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->showHideShadowToolStripMenuItem, 
				this->resetTransformToolStripMenuItem});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(182, 48);
			// 
			// showHideShadowToolStripMenuItem
			// 
			this->showHideShadowToolStripMenuItem->Name = L"showHideShadowToolStripMenuItem";
			this->showHideShadowToolStripMenuItem->Size = System::Drawing::Size(181, 22);
			this->showHideShadowToolStripMenuItem->Text = L"Show/Hide Shadow";
			this->showHideShadowToolStripMenuItem->Click += gcnew System::EventHandler(this, &ViewForm::showHideShadowToolStripMenuItem_Click);
			// 
			// resetTransformToolStripMenuItem
			// 
			this->resetTransformToolStripMenuItem->Name = L"resetTransformToolStripMenuItem";
			this->resetTransformToolStripMenuItem->Size = System::Drawing::Size(181, 22);
			this->resetTransformToolStripMenuItem->Text = L"Reset Transform";
			this->resetTransformToolStripMenuItem->Click += gcnew System::EventHandler(this, &ViewForm::resetTransformToolStripMenuItem_Click);
			// 
			// ViewForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(512, 512);
			this->ControlBox = false;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"ViewForm";
			this->Text = L"ViewForm";
			this->Activated += gcnew System::EventHandler(this, &ViewForm::ViewForm_Activated);
			this->Deactivate += gcnew System::EventHandler(this, &ViewForm::ViewForm_Deactivate);
			this->Shown += gcnew System::EventHandler(this, &ViewForm::ViewForm_Shown);
			this->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &ViewForm::ViewForm_MouseClick);
			this->contextMenuStrip1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private:
		bool AppStillIdle()
		{
			MSG msg;
			return !::PeekMessage(&msg, (HWND)(void*)(0), 0, 0, 0); 
		}
		System::Void OnApplicationIdle(System::Object^ sender,System::EventArgs^ e)
		{						
			while (AppStillIdle())
			{
				m_pFramework->Run();
			}			
		}
	private: System::Void ViewForm_Activated(System::Object^  sender, System::EventArgs^  e) {


				 if (m_pFramework)
				 {
					 m_pFramework->GetInput()->Acquire();
				 }
				 OutputDebugString(L"ViewForm_Activated");
			 }
	private: System::Void ViewForm_Deactivate(System::Object^  sender, System::EventArgs^  e) {
				 if (m_pFramework)
				 {
					 m_pFramework->GetInput()->UnAcquire();
				 }
				 OutputDebugString(L"ViewForm_Deactivate");
			 }
	private: System::Void ViewForm_Shown(System::Object^  sender, System::EventArgs^  e) {
				 OutputDebugString(L"ViewForm_Shown");

				 if (m_pFramework==NULL)
				 {
					 Application::Idle += gcnew System::EventHandler(this,&ViewForm::OnApplicationIdle);				
					 m_pFramework = new Framework("AssetViewer",false,this->ClientSize.Width,this->ClientSize.Height);
					 m_pFramework->m_hWndMain = (HWND)(void*)MdiParent->Handle;
					 m_pFramework->m_hWndPresent = (HWND)(void*)this->Handle;
					 if(!m_pFramework->Initialize())
					 {
						 Application::Exit();
					 }
					 m_pState = (State*)Sophia::cD3DFramework::m_pInstance->GetView();
				 }
			 }
	private: System::Void ViewForm_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void showHideShadowToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void showHideSkeletonToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void resetTransformToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
};

}
