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
	/// Form1�� ���� ����Դϴ�.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//
			
		}

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
	
		}

	private:
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>
		System::ComponentModel::Container ^components;
	private: System::Windows::Forms::TextBox^  textBox1;



#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(54, 41);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 21);
			this->textBox1->TabIndex = 0;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Form1::textBox1_TextChanged);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(264, 234);
			this->Controls->Add(this->textBox1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);
			this->PerformLayout();

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
};
}

