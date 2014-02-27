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
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;




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
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fileToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1280, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->openToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1280, 960);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
protected:
		virtual void OnShown(EventArgs^ e)  override 
		{
			m_pFramework->m_hWndMain = (HWND)(void*)this->Handle;
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
				m_pFramework->Run();
			}			
		}
	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

				 //Stream^ myStream;
				 OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

				 openFileDialog1->InitialDirectory = "c:\\";
				 openFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
				 openFileDialog1->FilterIndex = 2;
				 openFileDialog1->RestoreDirectory = true;

				 if ( openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK )
				 {
					 if ( openFileDialog1->OpenFile() != nullptr )
					 {
						 // Insert code to read the stream here.
						 //myStream->Close();
					 }
				 }

			 }
private: System::Void splitter1_SplitterMoved(System::Object^  sender, System::Windows::Forms::SplitterEventArgs^  e) {
		 }
};
}

