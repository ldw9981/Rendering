#pragma once

#pragma comment(lib,"User32.lib")

#include <windows.h>
#include "graphics/graphics.h"
#include "Graphics/World.h"
#include "Framework.h"

namespace AssetViewer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1에 대한 요약입니다.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: 생성자 코드를 여기에 추가합니다.
			//
			Application::Idle += gcnew System::EventHandler(this,&Form1::OnApplicationIdle);
			/*
			m_pGraphics = new Sophia::Graphics;
			m_pGraphicsWorld = new Sophia::World;
			*/
			m_pFramework = new Framework("AssetViewer",false,this->Size.Width,this->Size.Height);
		}

	protected:
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
			m_pFramework->Finalize();
			delete m_pFramework;
			m_pFramework = NULL;
			/*
			m_pGraphicsWorld->Finalize();
			delete m_pGraphicsWorld;

			m_pGraphics->Finalize();
			delete m_pGraphics;
			*/
		}

	private:
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>
		System::ComponentModel::Container ^components;

			 Framework*		m_pFramework;
//		Sophia::Graphics* m_pGraphics;
//		Sophia::World*	 m_pGraphicsWorld;
#pragma region Windows Form Designer generated code
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1008, 730);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);

		}
#pragma endregion
protected:
		virtual void OnShown(EventArgs^ e)  override 
		{
			m_pFramework->m_hWnd = (HWND)(void*)this->Handle;
			if(!m_pFramework->Initialize())
			{
				Application::Exit();
			}
		}
private:
		bool AppStillIdle()
		{
			MSG msg;
			return !::PeekMessage(&msg, (HWND)(Handle.ToPointer()), 0, 0, 0); 
		}
		System::Void OnApplicationIdle(System::Object^ sender,System::EventArgs^ e)
		{						
			while (AppStillIdle())
			{
				// Render a frame during idle time (no messages are waiting)
				//UpdateEnvironment();
				//Render3DEnvironment();
/*
				m_pGraphics->Begin();
//				m_pGraphicsWorld->ProcessRender();
				m_pGraphics->End();
				*/
				m_pFramework->Run();
			}			
		}
	};
}

