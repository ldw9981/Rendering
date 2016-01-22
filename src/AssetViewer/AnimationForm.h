#pragma once
#include "Graphics/SceneNode.h"

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
		
			m_pNode = NULL;
			m_pEntity = NULL;
			
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
	public: System::Windows::Forms::ListBox^  listAnimation;
	private: System::Windows::Forms::TextBox^  textStartTime;
	private: System::Windows::Forms::TextBox^  textEndTime;
	protected: 







	protected:
		
		State* m_pState;
		Sophia::Entity*	m_pEntity;
		Sophia::cSceneNode* m_pNode;
	private: System::Windows::Forms::TextBox^  textLength;
	private: System::Windows::Forms::TextBox^  textCutName;

	private: System::Windows::Forms::Button^  buttonCut;
	private: System::Windows::Forms::Button^  buttonDel;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::Label^  labelPartialWeight;
	private: System::Windows::Forms::TextBox^  textBox_PartialWeight;
	private: System::Windows::Forms::Button^  button_Partial;
	private: System::Windows::Forms::Button^  button_Base;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Button^  button_PartialStop;
	private: System::Windows::Forms::Button^  button_BaseStop;
	private: System::Windows::Forms::CheckBox^  checkBox_partialLoop;

	private: System::Windows::Forms::CheckBox^  checkBox_baseLoop;
	private: System::Windows::Forms::Label^  label1;
	public:
		System::Windows::Forms::CheckBox^  checkBox_showPartialWeight;
	private: System::Windows::Forms::ComboBox^  comboBoxPartial;
	public: 

	private: System::Windows::Forms::ComboBox^  comboBoxBase;
	public: 







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
			this->textLength = (gcnew System::Windows::Forms::TextBox());
			this->textCutName = (gcnew System::Windows::Forms::TextBox());
			this->buttonCut = (gcnew System::Windows::Forms::Button());
			this->buttonDel = (gcnew System::Windows::Forms::Button());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->comboBoxPartial = (gcnew System::Windows::Forms::ComboBox());
			this->checkBox_showPartialWeight = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_partialLoop = (gcnew System::Windows::Forms::CheckBox());
			this->button_PartialStop = (gcnew System::Windows::Forms::Button());
			this->button_Partial = (gcnew System::Windows::Forms::Button());
			this->textBox_PartialWeight = (gcnew System::Windows::Forms::TextBox());
			this->labelPartialWeight = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->comboBoxBase = (gcnew System::Windows::Forms::ComboBox());
			this->checkBox_baseLoop = (gcnew System::Windows::Forms::CheckBox());
			this->button_BaseStop = (gcnew System::Windows::Forms::Button());
			this->button_Base = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// listAnimation
			// 
			this->listAnimation->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listAnimation->FormattingEnabled = true;
			this->listAnimation->ItemHeight = 12;
			this->listAnimation->Location = System::Drawing::Point(0, 0);
			this->listAnimation->Name = L"listAnimation";
			this->listAnimation->Size = System::Drawing::Size(237, 239);
			this->listAnimation->TabIndex = 0;
			this->listAnimation->SelectedIndexChanged += gcnew System::EventHandler(this, &AnimationForm::listAnimation_OnSelectedIndexChanged);
			// 
			// textStartTime
			// 
			this->textStartTime->Location = System::Drawing::Point(126, 5);
			this->textStartTime->Name = L"textStartTime";
			this->textStartTime->Size = System::Drawing::Size(40, 21);
			this->textStartTime->TabIndex = 1;
			this->textStartTime->Text = L"0";
			this->textStartTime->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &AnimationForm::textStartTime_KeyPress);
			this->textStartTime->Leave += gcnew System::EventHandler(this, &AnimationForm::textStartTime_Leave);
			// 
			// textEndTime
			// 
			this->textEndTime->Location = System::Drawing::Point(174, 5);
			this->textEndTime->Name = L"textEndTime";
			this->textEndTime->Size = System::Drawing::Size(40, 21);
			this->textEndTime->TabIndex = 2;
			this->textEndTime->Text = L"0";
			this->textEndTime->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &AnimationForm::textEndTime_KeyPress);
			this->textEndTime->Leave += gcnew System::EventHandler(this, &AnimationForm::textEndTime_Leave);
			// 
			// textLength
			// 
			this->textLength->Location = System::Drawing::Point(49, 5);
			this->textLength->Name = L"textLength";
			this->textLength->Size = System::Drawing::Size(49, 21);
			this->textLength->TabIndex = 4;
			this->textLength->Text = L"0";
			// 
			// textCutName
			// 
			this->textCutName->Location = System::Drawing::Point(14, 230);
			this->textCutName->Name = L"textCutName";
			this->textCutName->Size = System::Drawing::Size(124, 21);
			this->textCutName->TabIndex = 5;
			// 
			// buttonCut
			// 
			this->buttonCut->Location = System::Drawing::Point(144, 230);
			this->buttonCut->Name = L"buttonCut";
			this->buttonCut->Size = System::Drawing::Size(38, 23);
			this->buttonCut->TabIndex = 6;
			this->buttonCut->Text = L"Cut";
			this->buttonCut->UseVisualStyleBackColor = true;
			this->buttonCut->Click += gcnew System::EventHandler(this, &AnimationForm::buttonCut_Click);
			// 
			// buttonDel
			// 
			this->buttonDel->Location = System::Drawing::Point(182, 230);
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
			this->splitContainer1->Panel1->Controls->Add(this->label1);
			this->splitContainer1->Panel1->Controls->Add(this->groupBox2);
			this->splitContainer1->Panel1->Controls->Add(this->groupBox1);
			this->splitContainer1->Panel1->Controls->Add(this->textLength);
			this->splitContainer1->Panel1->Controls->Add(this->buttonDel);
			this->splitContainer1->Panel1->Controls->Add(this->textStartTime);
			this->splitContainer1->Panel1->Controls->Add(this->buttonCut);
			this->splitContainer1->Panel1->Controls->Add(this->textEndTime);
			this->splitContainer1->Panel1->Controls->Add(this->textCutName);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->listAnimation);
			this->splitContainer1->Size = System::Drawing::Size(237, 500);
			this->splitContainer1->SplitterDistance = 257;
			this->splitContainer1->TabIndex = 8;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(34, 12);
			this->label1->TabIndex = 15;
			this->label1->Text = L"Time";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->comboBoxPartial);
			this->groupBox2->Controls->Add(this->checkBox_showPartialWeight);
			this->groupBox2->Controls->Add(this->checkBox_partialLoop);
			this->groupBox2->Controls->Add(this->button_PartialStop);
			this->groupBox2->Controls->Add(this->button_Partial);
			this->groupBox2->Controls->Add(this->textBox_PartialWeight);
			this->groupBox2->Controls->Add(this->labelPartialWeight);
			this->groupBox2->Location = System::Drawing::Point(13, 116);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(209, 108);
			this->groupBox2->TabIndex = 14;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Partial";
			// 
			// comboBoxPartial
			// 
			this->comboBoxPartial->FormattingEnabled = true;
			this->comboBoxPartial->Location = System::Drawing::Point(12, 16);
			this->comboBoxPartial->Name = L"comboBoxPartial";
			this->comboBoxPartial->Size = System::Drawing::Size(182, 20);
			this->comboBoxPartial->TabIndex = 17;
			this->comboBoxPartial->SelectedIndexChanged += gcnew System::EventHandler(this, &AnimationForm::comboBoxPartial_SelectedIndexChanged);
			// 
			// checkBox_showPartialWeight
			// 
			this->checkBox_showPartialWeight->AutoSize = true;
			this->checkBox_showPartialWeight->Location = System::Drawing::Point(9, 67);
			this->checkBox_showPartialWeight->Name = L"checkBox_showPartialWeight";
			this->checkBox_showPartialWeight->Size = System::Drawing::Size(173, 16);
			this->checkBox_showPartialWeight->TabIndex = 16;
			this->checkBox_showPartialWeight->Text = L"Show BlendWeight in Tree";
			this->checkBox_showPartialWeight->UseVisualStyleBackColor = true;
			// 
			// checkBox_partialLoop
			// 
			this->checkBox_partialLoop->AutoSize = true;
			this->checkBox_partialLoop->Location = System::Drawing::Point(9, 42);
			this->checkBox_partialLoop->Name = L"checkBox_partialLoop";
			this->checkBox_partialLoop->Size = System::Drawing::Size(52, 16);
			this->checkBox_partialLoop->TabIndex = 14;
			this->checkBox_partialLoop->Text = L"Loop";
			this->checkBox_partialLoop->UseVisualStyleBackColor = true;
			// 
			// button_PartialStop
			// 
			this->button_PartialStop->Location = System::Drawing::Point(151, 38);
			this->button_PartialStop->Name = L"button_PartialStop";
			this->button_PartialStop->Size = System::Drawing::Size(43, 23);
			this->button_PartialStop->TabIndex = 13;
			this->button_PartialStop->Text = L"Stop";
			this->button_PartialStop->UseVisualStyleBackColor = true;
			this->button_PartialStop->Click += gcnew System::EventHandler(this, &AnimationForm::button_PartialStop_Click);
			// 
			// button_Partial
			// 
			this->button_Partial->Location = System::Drawing::Point(99, 38);
			this->button_Partial->Name = L"button_Partial";
			this->button_Partial->Size = System::Drawing::Size(43, 23);
			this->button_Partial->TabIndex = 12;
			this->button_Partial->Text = L"Play";
			this->button_Partial->UseVisualStyleBackColor = true;
			this->button_Partial->Click += gcnew System::EventHandler(this, &AnimationForm::button_Partial_Click);
			// 
			// textBox_PartialWeight
			// 
			this->textBox_PartialWeight->Location = System::Drawing::Point(90, 83);
			this->textBox_PartialWeight->Name = L"textBox_PartialWeight";
			this->textBox_PartialWeight->Size = System::Drawing::Size(100, 21);
			this->textBox_PartialWeight->TabIndex = 10;
			this->textBox_PartialWeight->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &AnimationForm::textBox_PartialWeight_KeyDown);
			this->textBox_PartialWeight->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &AnimationForm::textBox_PartialWeight_KeyPress);
			// 
			// labelPartialWeight
			// 
			this->labelPartialWeight->AutoSize = true;
			this->labelPartialWeight->Location = System::Drawing::Point(7, 89);
			this->labelPartialWeight->Name = L"labelPartialWeight";
			this->labelPartialWeight->Size = System::Drawing::Size(74, 12);
			this->labelPartialWeight->TabIndex = 9;
			this->labelPartialWeight->Text = L"BlendWeight";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->comboBoxBase);
			this->groupBox1->Controls->Add(this->checkBox_baseLoop);
			this->groupBox1->Controls->Add(this->button_BaseStop);
			this->groupBox1->Controls->Add(this->button_Base);
			this->groupBox1->Location = System::Drawing::Point(14, 32);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(208, 78);
			this->groupBox1->TabIndex = 13;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Base";
			// 
			// comboBoxBase
			// 
			this->comboBoxBase->FormattingEnabled = true;
			this->comboBoxBase->Location = System::Drawing::Point(7, 20);
			this->comboBoxBase->Name = L"comboBoxBase";
			this->comboBoxBase->Size = System::Drawing::Size(187, 20);
			this->comboBoxBase->TabIndex = 16;
			this->comboBoxBase->SelectedIndexChanged += gcnew System::EventHandler(this, &AnimationForm::comboBoxBase_SelectedIndexChanged);
			// 
			// checkBox_baseLoop
			// 
			this->checkBox_baseLoop->AutoSize = true;
			this->checkBox_baseLoop->Location = System::Drawing::Point(7, 48);
			this->checkBox_baseLoop->Name = L"checkBox_baseLoop";
			this->checkBox_baseLoop->Size = System::Drawing::Size(52, 16);
			this->checkBox_baseLoop->TabIndex = 13;
			this->checkBox_baseLoop->Text = L"Loop";
			this->checkBox_baseLoop->UseVisualStyleBackColor = true;
			// 
			// button_BaseStop
			// 
			this->button_BaseStop->Location = System::Drawing::Point(151, 44);
			this->button_BaseStop->Name = L"button_BaseStop";
			this->button_BaseStop->Size = System::Drawing::Size(43, 23);
			this->button_BaseStop->TabIndex = 12;
			this->button_BaseStop->Text = L"Stop";
			this->button_BaseStop->UseVisualStyleBackColor = true;
			this->button_BaseStop->Click += gcnew System::EventHandler(this, &AnimationForm::button_BaseStop_Click);
			// 
			// button_Base
			// 
			this->button_Base->Location = System::Drawing::Point(97, 44);
			this->button_Base->Name = L"button_Base";
			this->button_Base->Size = System::Drawing::Size(44, 23);
			this->button_Base->TabIndex = 11;
			this->button_Base->Text = L"Play";
			this->button_Base->UseVisualStyleBackColor = true;
			this->button_Base->Click += gcnew System::EventHandler(this, &AnimationForm::button_Base_Click);
			// 
			// AnimationForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(237, 500);
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
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void OnShown(System::Object^  sender, System::EventArgs^  e);
	public:
		void Update(State* pState,Sophia::cSceneNode* pNode);
		void Clear();
		void MakeValidCutTime();
	private: void PlayAnimation();
	private: void UpdateText(bool modified);
	private: System::Void listAnimation_OnSelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void loop_OnCheckedChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void textStartTime_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
	private: System::Void textEndTime_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
	private: System::Void buttonCut_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void buttonDel_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void textBox_PartialWeight_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
private: System::Void textBox_PartialWeight_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
private: System::Void button_Base_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void button_Partial_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void button_BaseStop_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void button_PartialStop_Click(System::Object^  sender, System::EventArgs^  e);


private: System::Void textEndTime_Leave(System::Object^  sender, System::EventArgs^  e);
private: System::Void textStartTime_Leave(System::Object^  sender, System::EventArgs^  e);
private: System::Void comboBoxBase_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void comboBoxPartial_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
};
}
