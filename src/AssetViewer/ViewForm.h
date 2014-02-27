#pragma once
#include "graphics/graphics.h"
#include "Graphics/World.h"
#include "Framework.h"
#include "State.h"

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
			Application::Idle += gcnew System::EventHandler(this,&ViewForm::OnApplicationIdle);
			m_pFramework = new Framework("AssetViewer",false,this->Size.Width,this->Size.Height);
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

	private:
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>
		System::ComponentModel::Container ^components;
		Framework*		m_pFramework;
		State*			m_pState;
#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// ViewForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1024, 768);
			this->ControlBox = false;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"ViewForm";
			this->Text = L"ViewForm";
			this->ResumeLayout(false);

		}
#pragma endregion
	protected:
		virtual void OnShown(EventArgs^ e)  override 
		{
			m_pFramework->m_hWndMain = (HWND)(void*)MdiParent->Handle;
			m_pFramework->m_hWndPresent = (HWND)(void*)this->Handle;
			if(!m_pFramework->Initialize())
			{
				Application::Exit();
			}
			m_pState = (State*)Sophia::cD3DFramework::m_pInstance->GetView();
		}
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
	};
}
