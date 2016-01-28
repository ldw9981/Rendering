#pragma once
#include "SceneProperty.h"
#include "State.h"
#include "Graphics/SceneNode.h"


namespace AssetViewer {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// ScenePropertyForm에 대한 요약입니다.
	/// </summary>
	public ref class ScenePropertyForm : public System::Windows::Forms::Form
	{
	public:
		ScenePropertyForm(void)
		{
			InitializeComponent();
			//
			//TODO: 생성자 코드를 여기에 추가합니다.
			//
			propertyData = gcnew SceneProperty;
		}

	protected:
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		~ScenePropertyForm()
		{
			if (components)
			{
				delete components;
			}
			delete propertyData;
		}
	private: System::Windows::Forms::PropertyGrid^  propertyGrid1;
	protected: 

	private:
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>
		System::ComponentModel::Container ^components;
		SceneProperty^ propertyData;
		State* m_pState;

			 Sophia::cSceneNode* m_pNode;
#pragma region Windows Form Designer generated code
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
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
			this->propertyGrid1->Size = System::Drawing::Size(318, 312);
			this->propertyGrid1->TabIndex = 0;
			this->propertyGrid1->PropertyValueChanged += gcnew System::Windows::Forms::PropertyValueChangedEventHandler(this, &ScenePropertyForm::propertyGrid1_PropertyValueChanged);
			this->propertyGrid1->BindingContextChanged += gcnew System::EventHandler(this, &ScenePropertyForm::propertyGrid1_BindingContextChanged);
			// 
			// ScenePropertyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(318, 312);
			this->Controls->Add(this->propertyGrid1);
			this->MaximizeBox = false;
			this->Name = L"ScenePropertyForm";
			this->Text = L"ScenePropertyForm";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void propertyGrid1_BindingContextChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
	public: void Update(State* pState,Sophia::cSceneNode* pNode);
	private: System::Void propertyGrid1_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e);
	};
}
