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
	/// MaterialPropertyForm�� ���� ����Դϴ�.
	/// </summary>
	public ref class MaterialPropertyForm : public System::Windows::Forms::Form
	{
	public:
		MaterialPropertyForm(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//
			propertyData = gcnew MaterialProperty;
		}

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
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
	public: System::Windows::Forms::ComboBox^  comboBox1;

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
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->propertyGrid1 = (gcnew System::Windows::Forms::PropertyGrid());
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
			this->splitContainer1->Panel1->Controls->Add(this->comboBox1);
			this->splitContainer1->Panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MaterialPropertyForm::splitContainer1_Panel1_Paint);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->propertyGrid1);
			this->splitContainer1->Size = System::Drawing::Size(312, 480);
			this->splitContainer1->SplitterDistance = 25;
			this->splitContainer1->TabIndex = 0;
			// 
			// comboBox1
			// 
			this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(4, 4);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 20);
			this->comboBox1->TabIndex = 5;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MaterialPropertyForm::comboBox1_SelectedIndexChanged);
			// 
			// propertyGrid1
			// 
			this->propertyGrid1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->propertyGrid1->Location = System::Drawing::Point(0, 0);
			this->propertyGrid1->Name = L"propertyGrid1";
			this->propertyGrid1->Size = System::Drawing::Size(312, 451);
			this->propertyGrid1->TabIndex = 0;
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
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	public: void Update(State* pState,Sophia::cSceneNode* pNode);
	private: System::Void splitContainer1_Panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
			 }

private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
};
}

