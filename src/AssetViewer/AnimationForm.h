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
	/// AnimationForm�� ���� ����Դϴ�.
	/// </summary>
	public ref class AnimationForm : public System::Windows::Forms::Form
	{
	public:
		AnimationForm(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//
			m_selectedIndexLast = -1;
		}

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~AnimationForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^  listAnimation;
	protected: 

	private: System::Windows::Forms::TextBox^  textSkipStartTime;
	private: System::Windows::Forms::TextBox^  textEarlyEndTime;


	private: System::Windows::Forms::CheckBox^  checkBox1;

	protected: 
		int		m_selectedIndexLast;
		State* m_pState;
	private: System::Windows::Forms::TextBox^  textLength;
	protected: 
	private:
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		void InitializeComponent(void)
		{
			this->listAnimation = (gcnew System::Windows::Forms::ListBox());
			this->textSkipStartTime = (gcnew System::Windows::Forms::TextBox());
			this->textEarlyEndTime = (gcnew System::Windows::Forms::TextBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->textLength = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// listAnimation
			// 
			this->listAnimation->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->listAnimation->FormattingEnabled = true;
			this->listAnimation->ItemHeight = 12;
			this->listAnimation->Location = System::Drawing::Point(0, 35);
			this->listAnimation->Name = L"listAnimation";
			this->listAnimation->Size = System::Drawing::Size(212, 232);
			this->listAnimation->TabIndex = 0;
			this->listAnimation->SelectedIndexChanged += gcnew System::EventHandler(this, &AnimationForm::listAnimation_OnSelectedIndexChanged);
			// 
			// textSkipStartTime
			// 
			this->textSkipStartTime->Location = System::Drawing::Point(117, 5);
			this->textSkipStartTime->Name = L"textSkipStartTime";
			this->textSkipStartTime->Size = System::Drawing::Size(40, 21);
			this->textSkipStartTime->TabIndex = 1;
			this->textSkipStartTime->Text = L"0";
			this->textSkipStartTime->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &AnimationForm::textSkipStartTime_KeyPress);
			// 
			// textEarlyEndTime
			// 
			this->textEarlyEndTime->Location = System::Drawing::Point(165, 5);
			this->textEarlyEndTime->Name = L"textEarlyEndTime";
			this->textEarlyEndTime->Size = System::Drawing::Size(40, 21);
			this->textEarlyEndTime->TabIndex = 2;
			this->textEarlyEndTime->Text = L"0";
			this->textEarlyEndTime->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &AnimationForm::textEarlyEndTime_KeyPress);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(60, 7);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(52, 16);
			this->checkBox1->TabIndex = 3;
			this->checkBox1->Text = L"Loop";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &AnimationForm::loop_OnCheckedChanged);
			// 
			// textLength
			// 
			this->textLength->Location = System::Drawing::Point(9, 5);
			this->textLength->Name = L"textLength";
			this->textLength->Size = System::Drawing::Size(40, 21);
			this->textLength->TabIndex = 4;
			// 
			// AnimationForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(212, 267);
			this->ControlBox = false;
			this->Controls->Add(this->textLength);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->textEarlyEndTime);
			this->Controls->Add(this->textSkipStartTime);
			this->Controls->Add(this->listAnimation);
			this->MaximizeBox = false;
			this->Name = L"AnimationForm";
			this->Text = L"AnimationForm";
			this->Shown += gcnew System::EventHandler(this, &AnimationForm::OnShown);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
	private: System::Void OnShown(System::Object^  sender, System::EventArgs^  e);

	public:	void Update(State* pState);
	private: void PlayAnimation();
	
	private: System::Void listAnimation_OnSelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);

private: System::Void loop_OnCheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void textSkipStartTime_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
private: System::Void textEarlyEndTime_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
};
}
