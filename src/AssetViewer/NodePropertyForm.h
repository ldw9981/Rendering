#pragma once

namespace AssetViewer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// NodePropertyForm�� ���� ����Դϴ�.
	/// </summary>
	public ref class NodePropertyForm : public System::Windows::Forms::Form
	{
	public:
		NodePropertyForm(void)
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
		~NodePropertyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PropertyGrid^  propertyGrid1;
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
			this->propertyGrid1 = (gcnew System::Windows::Forms::PropertyGrid());
			this->SuspendLayout();
			// 
			// propertyGrid1
			// 
			this->propertyGrid1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->propertyGrid1->Location = System::Drawing::Point(0, 0);
			this->propertyGrid1->Name = L"propertyGrid1";
			this->propertyGrid1->Size = System::Drawing::Size(284, 262);
			this->propertyGrid1->TabIndex = 0;
			// 
			// NodePropertyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 262);
			this->ControlBox = false;
			this->Controls->Add(this->propertyGrid1);
			this->Name = L"NodePropertyForm";
			this->Text = L"NodePropertyForm";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
