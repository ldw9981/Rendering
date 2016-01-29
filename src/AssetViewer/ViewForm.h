#pragma once
#include "graphics/graphics.h"
#include "Graphics/World.h"
#include "Framework.h"
#include "State.h"
#include "Input/Input.h"
#include "CameraSettingForm.h"

namespace AssetViewer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// ViewForm에 대한 요약입니다.
	/// </summary>
	public ref class ViewForm : public System::Windows::Forms::Form
	{
	public:
		ViewForm(void)
		{
			InitializeComponent();
			//
			//TODO: 생성자 코드를 여기에 추가합니다.
			//
			m_pFramework = NULL;
			cameraSettingForm = nullptr;
		}

	protected:
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
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
			if (cameraSettingForm != nullptr)
			{
				delete cameraSettingForm;
				cameraSettingForm = nullptr;
			}
		}
	public:
		CameraSettingForm^		cameraSettingForm;
		Framework*		m_pFramework;
		State*			m_pState;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  showHideShadowToolStripMenuItem;



	private: System::Windows::Forms::ToolStripMenuItem^  resetTransformToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  cameraSettingsToolStripMenuItem;

	public: 
	private: System::ComponentModel::IContainer^  components;
	private:
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>

		
#pragma region Windows Form Designer generated code
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->showHideShadowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->resetTransformToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cameraSettingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->showHideShadowToolStripMenuItem, 
				this->resetTransformToolStripMenuItem, this->cameraSettingsToolStripMenuItem});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(203, 92);
			// 
			// showHideShadowToolStripMenuItem
			// 
			this->showHideShadowToolStripMenuItem->Checked = true;
			this->showHideShadowToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showHideShadowToolStripMenuItem->Name = L"showHideShadowToolStripMenuItem";
			this->showHideShadowToolStripMenuItem->Size = System::Drawing::Size(202, 22);
			this->showHideShadowToolStripMenuItem->Text = L"Show Shadow";
			this->showHideShadowToolStripMenuItem->Click += gcnew System::EventHandler(this, &ViewForm::showHideShadowToolStripMenuItem_Click);
			// 
			// resetTransformToolStripMenuItem
			// 
			this->resetTransformToolStripMenuItem->Name = L"resetTransformToolStripMenuItem";
			this->resetTransformToolStripMenuItem->Size = System::Drawing::Size(202, 22);
			this->resetTransformToolStripMenuItem->Text = L"Reset Transform";
			this->resetTransformToolStripMenuItem->Click += gcnew System::EventHandler(this, &ViewForm::resetTransformToolStripMenuItem_Click);
			// 
			// cameraSettingsToolStripMenuItem
			// 
			this->cameraSettingsToolStripMenuItem->Name = L"cameraSettingsToolStripMenuItem";
			this->cameraSettingsToolStripMenuItem->Size = System::Drawing::Size(202, 22);
			this->cameraSettingsToolStripMenuItem->Text = L"Light - Camera Settings";
			this->cameraSettingsToolStripMenuItem->Click += gcnew System::EventHandler(this, &ViewForm::cameraSettingsToolStripMenuItem_Click);
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
		System::Void OnApplicationIdle(System::Object^ sender,System::EventArgs^ e);
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
	private: System::Void resetTransformToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void cameraSettingsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	 private: System::Void cameraSettingsForm_Closed(System::Object^  sender, System::EventArgs^  e);
};

}
