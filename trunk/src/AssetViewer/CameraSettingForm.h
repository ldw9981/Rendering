#pragma once

class State;

namespace AssetViewer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// CameraSettingForm에 대한 요약입니다.
	/// </summary>
	public ref class CameraSettingForm : public System::Windows::Forms::Form
	{
	public:
		CameraSettingForm(void)
		{
			InitializeComponent();
			//
			//TODO: 생성자 코드를 여기에 추가합니다.
			//
			m_pState = NULL;
		}

	protected:
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		~CameraSettingForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  textBoxCamPosX;
	private: System::Windows::Forms::TextBox^  textBoxCamPosY;
	private: System::Windows::Forms::TextBox^  textBoxCamPosZ;
	private: System::Windows::Forms::TextBox^  textBoxLookAtPosX;
	private: System::Windows::Forms::TextBox^  textBoxLookAtPosY;
	private: System::Windows::Forms::TextBox^  textBoxLookAtPosZ;
	private: System::Windows::Forms::TextBox^  textBoxZScrollSize;







	public:
		State* m_pState;
	private:
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBoxCamPosX = (gcnew System::Windows::Forms::TextBox());
			this->textBoxCamPosY = (gcnew System::Windows::Forms::TextBox());
			this->textBoxCamPosZ = (gcnew System::Windows::Forms::TextBox());
			this->textBoxLookAtPosX = (gcnew System::Windows::Forms::TextBox());
			this->textBoxLookAtPosY = (gcnew System::Windows::Forms::TextBox());
			this->textBoxLookAtPosZ = (gcnew System::Windows::Forms::TextBox());
			this->textBoxZScrollSize = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(9, 12);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(99, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Camera Position";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 40);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(93, 12);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Loot At Position";
			this->label2->Click += gcnew System::EventHandler(this, &CameraSettingForm::label2_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(12, 67);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(78, 12);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Z Scroll Size";
			// 
			// textBoxCamPosX
			// 
			this->textBoxCamPosX->Location = System::Drawing::Point(114, 9);
			this->textBoxCamPosX->Name = L"textBoxCamPosX";
			this->textBoxCamPosX->Size = System::Drawing::Size(100, 21);
			this->textBoxCamPosX->TabIndex = 3;
			this->textBoxCamPosX->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CameraSettingForm::textBoxCamPosX_KeyDown);
			// 
			// textBoxCamPosY
			// 
			this->textBoxCamPosY->Location = System::Drawing::Point(221, 9);
			this->textBoxCamPosY->Name = L"textBoxCamPosY";
			this->textBoxCamPosY->Size = System::Drawing::Size(100, 21);
			this->textBoxCamPosY->TabIndex = 4;
			this->textBoxCamPosY->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CameraSettingForm::textBoxCamPosY_KeyDown);
			// 
			// textBoxCamPosZ
			// 
			this->textBoxCamPosZ->Location = System::Drawing::Point(328, 9);
			this->textBoxCamPosZ->Name = L"textBoxCamPosZ";
			this->textBoxCamPosZ->Size = System::Drawing::Size(100, 21);
			this->textBoxCamPosZ->TabIndex = 5;
			this->textBoxCamPosZ->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CameraSettingForm::textBoxCamPosZ_KeyDown);
			// 
			// textBoxLookAtPosX
			// 
			this->textBoxLookAtPosX->Location = System::Drawing::Point(114, 37);
			this->textBoxLookAtPosX->Name = L"textBoxLookAtPosX";
			this->textBoxLookAtPosX->Size = System::Drawing::Size(100, 21);
			this->textBoxLookAtPosX->TabIndex = 6;
			// 
			// textBoxLookAtPosY
			// 
			this->textBoxLookAtPosY->Location = System::Drawing::Point(221, 37);
			this->textBoxLookAtPosY->Name = L"textBoxLookAtPosY";
			this->textBoxLookAtPosY->Size = System::Drawing::Size(100, 21);
			this->textBoxLookAtPosY->TabIndex = 7;
			// 
			// textBoxLookAtPosZ
			// 
			this->textBoxLookAtPosZ->Location = System::Drawing::Point(328, 37);
			this->textBoxLookAtPosZ->Name = L"textBoxLookAtPosZ";
			this->textBoxLookAtPosZ->Size = System::Drawing::Size(100, 21);
			this->textBoxLookAtPosZ->TabIndex = 8;
			// 
			// textBoxZScrollSize
			// 
			this->textBoxZScrollSize->Location = System::Drawing::Point(114, 64);
			this->textBoxZScrollSize->Name = L"textBoxZScrollSize";
			this->textBoxZScrollSize->Size = System::Drawing::Size(100, 21);
			this->textBoxZScrollSize->TabIndex = 9;
			this->textBoxZScrollSize->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CameraSettingForm::textBoxZScrollSize_KeyDown);
			// 
			// CameraSettingForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(450, 105);
			this->Controls->Add(this->textBoxZScrollSize);
			this->Controls->Add(this->textBoxLookAtPosZ);
			this->Controls->Add(this->textBoxLookAtPosY);
			this->Controls->Add(this->textBoxLookAtPosX);
			this->Controls->Add(this->textBoxCamPosZ);
			this->Controls->Add(this->textBoxCamPosY);
			this->Controls->Add(this->textBoxCamPosX);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"CameraSettingForm";
			this->Text = L"CameraSettingForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
			 }

	public:	void Update(State* pState);
private: System::Void textBoxCamPosX_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
private: System::Void textBoxCamPosX_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
private: System::Void textBoxCamPosY_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
private: System::Void textBoxCamPosZ_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
private: System::Void textBoxZScrollSize_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
};
}
