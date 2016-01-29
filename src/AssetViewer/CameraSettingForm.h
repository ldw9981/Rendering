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
	private: System::Windows::Forms::TextBox^  textBoxUpVectorZ;
	private: System::Windows::Forms::TextBox^  textBoxUpVectorY;
	private: System::Windows::Forms::TextBox^  textBoxUpVectorX;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::TextBox^  textDirectionLightX;
	private: System::Windows::Forms::TextBox^  textDirectionLightY;
	private: System::Windows::Forms::TextBox^  textDirectionLightZ;



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
			this->textBoxUpVectorZ = (gcnew System::Windows::Forms::TextBox());
			this->textBoxUpVectorY = (gcnew System::Windows::Forms::TextBox());
			this->textBoxUpVectorX = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->textDirectionLightX = (gcnew System::Windows::Forms::TextBox());
			this->textDirectionLightY = (gcnew System::Windows::Forms::TextBox());
			this->textDirectionLightZ = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(21, 26);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(76, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Eye Position";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(21, 51);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(93, 12);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Loot At Position";
			this->label2->Click += gcnew System::EventHandler(this, &CameraSettingForm::label2_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(21, 109);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(78, 12);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Z Scroll Size";
			// 
			// textBoxCamPosX
			// 
			this->textBoxCamPosX->Location = System::Drawing::Point(123, 23);
			this->textBoxCamPosX->Name = L"textBoxCamPosX";
			this->textBoxCamPosX->Size = System::Drawing::Size(50, 21);
			this->textBoxCamPosX->TabIndex = 3;
			this->textBoxCamPosX->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CameraSettingForm::textBoxCamPos_KeyDown);
			this->textBoxCamPosX->Leave += gcnew System::EventHandler(this, &CameraSettingForm::textBoxCamPos_Leave);
			// 
			// textBoxCamPosY
			// 
			this->textBoxCamPosY->Location = System::Drawing::Point(181, 23);
			this->textBoxCamPosY->Name = L"textBoxCamPosY";
			this->textBoxCamPosY->Size = System::Drawing::Size(50, 21);
			this->textBoxCamPosY->TabIndex = 4;
			this->textBoxCamPosY->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CameraSettingForm::textBoxCamPos_KeyDown);
			this->textBoxCamPosY->Leave += gcnew System::EventHandler(this, &CameraSettingForm::textBoxCamPos_Leave);
			// 
			// textBoxCamPosZ
			// 
			this->textBoxCamPosZ->Location = System::Drawing::Point(240, 23);
			this->textBoxCamPosZ->Name = L"textBoxCamPosZ";
			this->textBoxCamPosZ->Size = System::Drawing::Size(50, 21);
			this->textBoxCamPosZ->TabIndex = 5;
			this->textBoxCamPosZ->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CameraSettingForm::textBoxCamPos_KeyDown);
			this->textBoxCamPosZ->Leave += gcnew System::EventHandler(this, &CameraSettingForm::textBoxCamPos_Leave);
			// 
			// textBoxLookAtPosX
			// 
			this->textBoxLookAtPosX->Location = System::Drawing::Point(123, 51);
			this->textBoxLookAtPosX->Name = L"textBoxLookAtPosX";
			this->textBoxLookAtPosX->Size = System::Drawing::Size(50, 21);
			this->textBoxLookAtPosX->TabIndex = 6;
			// 
			// textBoxLookAtPosY
			// 
			this->textBoxLookAtPosY->Location = System::Drawing::Point(181, 51);
			this->textBoxLookAtPosY->Name = L"textBoxLookAtPosY";
			this->textBoxLookAtPosY->Size = System::Drawing::Size(50, 21);
			this->textBoxLookAtPosY->TabIndex = 7;
			// 
			// textBoxLookAtPosZ
			// 
			this->textBoxLookAtPosZ->Location = System::Drawing::Point(240, 51);
			this->textBoxLookAtPosZ->Name = L"textBoxLookAtPosZ";
			this->textBoxLookAtPosZ->Size = System::Drawing::Size(50, 21);
			this->textBoxLookAtPosZ->TabIndex = 8;
			this->textBoxLookAtPosZ->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CameraSettingForm::textBoxLookAtPos_KeyDown);
			this->textBoxLookAtPosZ->Leave += gcnew System::EventHandler(this, &CameraSettingForm::textBoxLookAtPos_Leave);
			// 
			// textBoxZScrollSize
			// 
			this->textBoxZScrollSize->Location = System::Drawing::Point(123, 106);
			this->textBoxZScrollSize->Name = L"textBoxZScrollSize";
			this->textBoxZScrollSize->Size = System::Drawing::Size(50, 21);
			this->textBoxZScrollSize->TabIndex = 9;
			this->textBoxZScrollSize->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CameraSettingForm::textBoxZScrollSize_KeyDown);
			// 
			// textBoxUpVectorZ
			// 
			this->textBoxUpVectorZ->Location = System::Drawing::Point(240, 77);
			this->textBoxUpVectorZ->Name = L"textBoxUpVectorZ";
			this->textBoxUpVectorZ->Size = System::Drawing::Size(50, 21);
			this->textBoxUpVectorZ->TabIndex = 13;
			// 
			// textBoxUpVectorY
			// 
			this->textBoxUpVectorY->Location = System::Drawing::Point(181, 77);
			this->textBoxUpVectorY->Name = L"textBoxUpVectorY";
			this->textBoxUpVectorY->Size = System::Drawing::Size(50, 21);
			this->textBoxUpVectorY->TabIndex = 12;
			// 
			// textBoxUpVectorX
			// 
			this->textBoxUpVectorX->Location = System::Drawing::Point(123, 77);
			this->textBoxUpVectorX->Name = L"textBoxUpVectorX";
			this->textBoxUpVectorX->Size = System::Drawing::Size(50, 21);
			this->textBoxUpVectorX->TabIndex = 11;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(21, 80);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(60, 12);
			this->label4->TabIndex = 10;
			this->label4->Text = L"Up Vector";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->textBoxZScrollSize);
			this->groupBox1->Controls->Add(this->textBoxUpVectorZ);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->textBoxUpVectorY);
			this->groupBox1->Controls->Add(this->textBoxCamPosX);
			this->groupBox1->Controls->Add(this->textBoxUpVectorX);
			this->groupBox1->Controls->Add(this->textBoxCamPosY);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->textBoxCamPosZ);
			this->groupBox1->Controls->Add(this->textBoxLookAtPosX);
			this->groupBox1->Controls->Add(this->textBoxLookAtPosZ);
			this->groupBox1->Controls->Add(this->textBoxLookAtPosY);
			this->groupBox1->Location = System::Drawing::Point(12, 73);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(303, 147);
			this->groupBox1->TabIndex = 14;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Camera";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->textDirectionLightX);
			this->groupBox2->Controls->Add(this->textDirectionLightY);
			this->groupBox2->Controls->Add(this->textDirectionLightZ);
			this->groupBox2->Location = System::Drawing::Point(12, 13);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(303, 56);
			this->groupBox2->TabIndex = 15;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Direction Light";
			// 
			// textDirectionLightX
			// 
			this->textDirectionLightX->Location = System::Drawing::Point(123, 19);
			this->textDirectionLightX->Name = L"textDirectionLightX";
			this->textDirectionLightX->Size = System::Drawing::Size(50, 21);
			this->textDirectionLightX->TabIndex = 14;
			// 
			// textDirectionLightY
			// 
			this->textDirectionLightY->Location = System::Drawing::Point(181, 19);
			this->textDirectionLightY->Name = L"textDirectionLightY";
			this->textDirectionLightY->Size = System::Drawing::Size(50, 21);
			this->textDirectionLightY->TabIndex = 15;
			// 
			// textDirectionLightZ
			// 
			this->textDirectionLightZ->Location = System::Drawing::Point(240, 19);
			this->textDirectionLightZ->Name = L"textDirectionLightZ";
			this->textDirectionLightZ->Size = System::Drawing::Size(50, 21);
			this->textDirectionLightZ->TabIndex = 16;
			this->textDirectionLightZ->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CameraSettingForm::textDirectionLight_KeyDown);
			this->textDirectionLightZ->Leave += gcnew System::EventHandler(this, &CameraSettingForm::textDirectionLight_Leave);
			// 
			// CameraSettingForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(327, 248);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"CameraSettingForm";
			this->Text = L"LightCameraSettingForm";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
			 }

	public:	
		void UpdateForm(State* pState);
		void UpdateCamera();

private: System::Void textBoxCamPos_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
private: System::Void textBoxZScrollSize_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
private: System::Void textBoxCamPos_Leave(System::Object^  sender, System::EventArgs^  e);

private: System::Void textDirectionLight_Leave(System::Object^  sender, System::EventArgs^  e);
private: System::Void textDirectionLight_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
private: System::Void textBoxLookAtPos_Leave(System::Object^  sender, System::EventArgs^  e);
private: System::Void textBoxLookAtPos_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
};
}
