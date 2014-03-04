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
	/// AnimationForm에 대한 요약입니다.
	/// </summary>
	public ref class AnimationForm : public System::Windows::Forms::Form
	{
	public:
		AnimationForm(void)
		{
			InitializeComponent();
			//
			//TODO: 생성자 코드를 여기에 추가합니다.
			//
			m_selectedIndexLast = -1;
		}

	protected:
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		~AnimationForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^  listAnimation;
	private: System::Windows::Forms::TextBox^  textStartTime;
	private: System::Windows::Forms::TextBox^  textEndTime;
	protected: 





	private: System::Windows::Forms::CheckBox^  checkBox1;

	protected: 
		int		m_selectedIndexLast;
		State* m_pState;
	private: System::Windows::Forms::TextBox^  textLength;
	private: System::Windows::Forms::TextBox^  textCutName;

	private: System::Windows::Forms::Button^  buttonCut;
	private: System::Windows::Forms::Button^  buttonDel;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;


	protected: 
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
			this->listAnimation = (gcnew System::Windows::Forms::ListBox());
			this->textStartTime = (gcnew System::Windows::Forms::TextBox());
			this->textEndTime = (gcnew System::Windows::Forms::TextBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->textLength = (gcnew System::Windows::Forms::TextBox());
			this->textCutName = (gcnew System::Windows::Forms::TextBox());
			this->buttonCut = (gcnew System::Windows::Forms::Button());
			this->buttonDel = (gcnew System::Windows::Forms::Button());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->SuspendLayout();
			// 
			// listAnimation
			// 
			this->listAnimation->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listAnimation->FormattingEnabled = true;
			this->listAnimation->ItemHeight = 12;
			this->listAnimation->Location = System::Drawing::Point(0, 0);
			this->listAnimation->Name = L"listAnimation";
			this->listAnimation->Size = System::Drawing::Size(258, 441);
			this->listAnimation->TabIndex = 0;
			this->listAnimation->SelectedIndexChanged += gcnew System::EventHandler(this, &AnimationForm::listAnimation_OnSelectedIndexChanged);
			// 
			// textStartTime
			// 
			this->textStartTime->Location = System::Drawing::Point(120, 12);
			this->textStartTime->Name = L"textStartTime";
			this->textStartTime->Size = System::Drawing::Size(40, 21);
			this->textStartTime->TabIndex = 1;
			this->textStartTime->Text = L"0";
			this->textStartTime->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &AnimationForm::textStartTime_KeyPress);
			// 
			// textEndTime
			// 
			this->textEndTime->Location = System::Drawing::Point(168, 12);
			this->textEndTime->Name = L"textEndTime";
			this->textEndTime->Size = System::Drawing::Size(40, 21);
			this->textEndTime->TabIndex = 2;
			this->textEndTime->Text = L"0";
			this->textEndTime->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &AnimationForm::textEndTime_KeyPress);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(63, 14);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(52, 16);
			this->checkBox1->TabIndex = 3;
			this->checkBox1->Text = L"Loop";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &AnimationForm::loop_OnCheckedChanged);
			// 
			// textLength
			// 
			this->textLength->Location = System::Drawing::Point(12, 12);
			this->textLength->Name = L"textLength";
			this->textLength->Size = System::Drawing::Size(40, 21);
			this->textLength->TabIndex = 4;
			// 
			// textCutName
			// 
			this->textCutName->Location = System::Drawing::Point(12, 40);
			this->textCutName->Name = L"textCutName";
			this->textCutName->Size = System::Drawing::Size(103, 21);
			this->textCutName->TabIndex = 5;
			// 
			// buttonCut
			// 
			this->buttonCut->Location = System::Drawing::Point(120, 40);
			this->buttonCut->Name = L"buttonCut";
			this->buttonCut->Size = System::Drawing::Size(38, 23);
			this->buttonCut->TabIndex = 6;
			this->buttonCut->Text = L"Cut";
			this->buttonCut->UseVisualStyleBackColor = true;
			this->buttonCut->Click += gcnew System::EventHandler(this, &AnimationForm::buttonCut_Click);
			// 
			// buttonDel
			// 
			this->buttonDel->Location = System::Drawing::Point(168, 40);
			this->buttonDel->Name = L"buttonDel";
			this->buttonDel->Size = System::Drawing::Size(40, 23);
			this->buttonDel->TabIndex = 7;
			this->buttonDel->Text = L"Del";
			this->buttonDel->UseVisualStyleBackColor = true;
			this->buttonDel->Click += gcnew System::EventHandler(this, &AnimationForm::buttonDel_Click);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->AutoScroll = true;
			this->splitContainer1->Panel1->Controls->Add(this->textLength);
			this->splitContainer1->Panel1->Controls->Add(this->buttonDel);
			this->splitContainer1->Panel1->Controls->Add(this->textStartTime);
			this->splitContainer1->Panel1->Controls->Add(this->buttonCut);
			this->splitContainer1->Panel1->Controls->Add(this->textEndTime);
			this->splitContainer1->Panel1->Controls->Add(this->textCutName);
			this->splitContainer1->Panel1->Controls->Add(this->checkBox1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->listAnimation);
			this->splitContainer1->Size = System::Drawing::Size(258, 521);
			this->splitContainer1->SplitterDistance = 76;
			this->splitContainer1->TabIndex = 8;
			// 
			// AnimationForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(258, 521);
			this->ControlBox = false;
			this->Controls->Add(this->splitContainer1);
			this->MaximizeBox = false;
			this->Name = L"AnimationForm";
			this->Text = L"Animation";
			this->Shown += gcnew System::EventHandler(this, &AnimationForm::OnShown);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
	private: System::Void OnShown(System::Object^  sender, System::EventArgs^  e);

	public:	void Update(State* pState);
	private: void PlayAnimation();
	private: void UpdateText(bool modified);
	private: System::Void listAnimation_OnSelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);

private: System::Void loop_OnCheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void textStartTime_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
private: System::Void textEndTime_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);

private: System::Void buttonCut_Click(System::Object^  sender, System::EventArgs^  e);

private: System::Void buttonDel_Click(System::Object^  sender, System::EventArgs^  e);
};
}
