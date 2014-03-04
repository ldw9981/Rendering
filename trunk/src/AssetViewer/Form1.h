#pragma once

#pragma comment(lib,"User32.lib")

#include <windows.h>
#include "graphics/graphics.h"
#include "Graphics/World.h"
#include "Framework.h"
#include "SceneProperty.h"



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
	
		}
	private: System::ComponentModel::IContainer^  components;
	protected: 
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  cutAndPushBackToolStripMenuItem;

	private: System::Windows::Forms::PropertyGrid^  propertyGrid1;

	private:
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>


			 SceneProperty^ sceneProperty;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->cutAndPushBackToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->propertyGrid1 = (gcnew System::Windows::Forms::PropertyGrid());
			this->contextMenuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->cutAndPushBackToolStripMenuItem});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(167, 26);
			// 
			// cutAndPushBackToolStripMenuItem
			// 
			this->cutAndPushBackToolStripMenuItem->Name = L"cutAndPushBackToolStripMenuItem";
			this->cutAndPushBackToolStripMenuItem->Size = System::Drawing::Size(166, 22);
			this->cutAndPushBackToolStripMenuItem->Text = L"CutAndPushBack";
			// 
			// propertyGrid1
			// 
			this->propertyGrid1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->propertyGrid1->Location = System::Drawing::Point(0, 0);
			this->propertyGrid1->Name = L"propertyGrid1";
			this->propertyGrid1->Size = System::Drawing::Size(766, 444);
			this->propertyGrid1->TabIndex = 3;
			this->propertyGrid1->Tag = L"SceneProperty";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(766, 444);
			this->Controls->Add(this->propertyGrid1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->contextMenuStrip1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
protected:

private:

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
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 
		 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {

			 sceneProperty = gcnew SceneProperty;
			
			 sceneProperty->translation0_x = 0.3f;

			propertyGrid1->SelectedObject = sceneProperty;
			propertyGrid1->ExpandAllGridItems();

		 }
private: System::Void listBox1_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if( e->Button == System::Windows::Forms::MouseButtons::Right)
			 {
				 contextMenuStrip1->Show();
			 }
		 }
private: System::Void listBox1_Click(System::Object^  sender, System::EventArgs^  e) {
			 contextMenuStrip1->Show();
		 }
private: System::Void listBox1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if( e->Button == System::Windows::Forms::MouseButtons::Right)
			 {
				 contextMenuStrip1->Show();
			 }
		 }
};
}

