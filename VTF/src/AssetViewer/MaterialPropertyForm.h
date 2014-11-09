#pragma once
#include "MaterialProperty.h"
#include "State.h"
#include "Scene/SceneNode.h"

namespace AssetViewer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MaterialPropertyForm에 대한 요약입니다.
	/// </summary>
	public ref class MaterialPropertyForm : public System::Windows::Forms::Form
	{
	public:
		MaterialPropertyForm(void)
		{
			InitializeComponent();
			//
			//TODO: 생성자 코드를 여기에 추가합니다.
			//
			propertyData = gcnew MaterialProperty;
		}

	protected:
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		~MaterialPropertyForm()
		{
			if (components)
			{
				delete components;
			}
			delete propertyData;
		}

	protected: 
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::PropertyGrid^  propertyGrid1;
	private: MaterialProperty^ propertyData;
	protected: Sophia::cSceneNode* m_pNode;

	private: System::Windows::Forms::Label^  label2;
	public: 
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBoxSubIndex;

	private: System::Windows::Forms::TextBox^  textBoxRefIndex;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  textBoxTotal;



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
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBoxSubIndex = (gcnew System::Windows::Forms::TextBox());
			this->textBoxRefIndex = (gcnew System::Windows::Forms::TextBox());
			this->propertyGrid1 = (gcnew System::Windows::Forms::PropertyGrid());
			this->textBoxTotal = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->SuspendLayout();
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
			this->splitContainer1->Panel1->Controls->Add(this->label3);
			this->splitContainer1->Panel1->Controls->Add(this->textBoxTotal);
			this->splitContainer1->Panel1->Controls->Add(this->label2);
			this->splitContainer1->Panel1->Controls->Add(this->label1);
			this->splitContainer1->Panel1->Controls->Add(this->textBoxSubIndex);
			this->splitContainer1->Panel1->Controls->Add(this->textBoxRefIndex);
			this->splitContainer1->Panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MaterialPropertyForm::splitContainer1_Panel1_Paint);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->propertyGrid1);
			this->splitContainer1->Size = System::Drawing::Size(312, 480);
			this->splitContainer1->SplitterDistance = 25;
			this->splitContainer1->TabIndex = 0;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(190, 6);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(62, 12);
			this->label2->TabIndex = 9;
			this->label2->Text = L"Sub Index";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(95, 6);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(58, 12);
			this->label1->TabIndex = 8;
			this->label1->Text = L"Ref Index";
			// 
			// textBoxSubIndex
			// 
			this->textBoxSubIndex->Location = System::Drawing::Point(258, 2);
			this->textBoxSubIndex->Name = L"textBoxSubIndex";
			this->textBoxSubIndex->Size = System::Drawing::Size(28, 21);
			this->textBoxSubIndex->TabIndex = 7;
			// 
			// textBoxRefIndex
			// 
			this->textBoxRefIndex->Location = System::Drawing::Point(159, 3);
			this->textBoxRefIndex->Name = L"textBoxRefIndex";
			this->textBoxRefIndex->Size = System::Drawing::Size(25, 21);
			this->textBoxRefIndex->TabIndex = 6;
			// 
			// propertyGrid1
			// 
			this->propertyGrid1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->propertyGrid1->Location = System::Drawing::Point(0, 0);
			this->propertyGrid1->Name = L"propertyGrid1";
			this->propertyGrid1->Size = System::Drawing::Size(312, 451);
			this->propertyGrid1->TabIndex = 0;
			this->propertyGrid1->PropertyValueChanged += gcnew System::Windows::Forms::PropertyValueChangedEventHandler(this, &MaterialPropertyForm::propertyGrid1_PropertyValueChanged);
			// 
			// textBoxTotal
			// 
			this->textBoxTotal->Location = System::Drawing::Point(56, 3);
			this->textBoxTotal->Name = L"textBoxTotal";
			this->textBoxTotal->Size = System::Drawing::Size(33, 21);
			this->textBoxTotal->TabIndex = 10;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(12, 6);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(33, 12);
			this->label3->TabIndex = 11;
			this->label3->Text = L"Total";
			// 
			// MaterialPropertyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(312, 480);
			this->Controls->Add(this->splitContainer1);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"MaterialPropertyForm";
			this->Text = L"MaterialPropertyForm";
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	public: void Update(State* pState,Sophia::cSceneNode* pNode);
	private: System::Void splitContainer1_Panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
			 }

private: System::Void propertyGrid1_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e);
};
}

