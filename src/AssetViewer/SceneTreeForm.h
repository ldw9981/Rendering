#pragma once
#include "Framework.h"
#include "State.h"
#include "Scene/SceneNode.h"
#include "Resource.h"

namespace AssetViewer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// SceneTreeForm에 대한 요약입니다.
	/// </summary>
	public ref class SceneTreeForm : public System::Windows::Forms::Form
	{
	public:
		SceneTreeForm(void)
		{
			InitializeComponent();
			//
			//TODO: 생성자 코드를 여기에 추가합니다.
			//
					
			Reflection::Assembly^ pxAssembly = Reflection::Assembly::GetExecutingAssembly();
			String^ pxResName = pxAssembly->GetName()->Name + ".SceneTreeForm"; //Note: add your resourcefile name here, i.e-> ".MyResourceFile" as it appears in solution explorer, without it's extension
			auto s_pxResourceManager = (gcnew Resources::ResourceManager(pxResName,pxAssembly));

			//pictureBox1->Image =  //note: this is the exact name from the resx, not the filename or name in solution explorer. By default it's name in the resx won't contain it's extension.

			imageList.Images->Add((cli::safe_cast<Drawing::Bitmap^>(s_pxResourceManager->GetObject("scenetree_root"))));
			imageList.Images->Add((cli::safe_cast<Drawing::Bitmap^>(s_pxResourceManager->GetObject("scenetree_scene"))));
			imageList.Images->Add((cli::safe_cast<Drawing::Bitmap^>(s_pxResourceManager->GetObject("scenetree_mesh"))));
			imageList.Images->Add((cli::safe_cast<Drawing::Bitmap^>(s_pxResourceManager->GetObject("scenetree_skin"))));
			imageList.Images->Add((cli::safe_cast<Drawing::Bitmap^>(s_pxResourceManager->GetObject("scenetree_skeleton"))));
			treeView1->ImageList = %imageList;			
			tagFont = gcnew System::Drawing::Font( "Helvetica",8,FontStyle::Bold );
		}

	protected:
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		~SceneTreeForm()
		{
			if (components)
			{
				delete components;
			}
			if ( tagFont != nullptr )
			{
				delete tagFont;
			}

		}

	public:
	
	protected: 
		State*			m_pState;
		Sophia::Entity*	m_pEntity;
		ImageList imageList;
		int				m_animationIndex;
		bool			m_showWeight;
	private:
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>
		System::ComponentModel::Container ^components;
	public:
		System::Windows::Forms::TreeView^  treeView1;

		// Create a Font object for the node tags.
		System::Drawing::Font^ tagFont;

			 
#pragma region Windows Form Designer generated code
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
		/// </summary>
		void InitializeComponent(void)
		{
			this->treeView1 = (gcnew System::Windows::Forms::TreeView());
			this->SuspendLayout();
			// 
			// treeView1
			// 
			this->treeView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->treeView1->DrawMode = System::Windows::Forms::TreeViewDrawMode::OwnerDrawText;
			this->treeView1->HideSelection = false;
			this->treeView1->Location = System::Drawing::Point(0, 0);
			this->treeView1->Name = L"treeView1";
			this->treeView1->Size = System::Drawing::Size(240, 474);
			this->treeView1->TabIndex = 0;
			this->treeView1->DrawNode += gcnew System::Windows::Forms::DrawTreeNodeEventHandler(this, &SceneTreeForm::DrawTreeNodeHighlightSelectedEvenWithoutFocus);
			this->treeView1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &SceneTreeForm::treeview_OnMouseClick);
			// 
			// SceneTreeForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;		
			this->ControlBox = false;
			this->Controls->Add(this->treeView1);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"SceneTreeForm";
			this->Text = L"SceneTreeForm";
			this->Shown += gcnew System::EventHandler(this, &SceneTreeForm::OnShown);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: 
		System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }	
	private: 
		System::Void OnMouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		 }
	private: 
		System::Void treeview_OnMouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {			 
		 }
	private: 
		System::Void OnShown(System::Object^  sender, System::EventArgs^  e) {

			 
		 }

	public:	
		void Clear();
		void SetAnimationIndex(int index);
		void Update(State* pState);
	public: 
		TreeNode^ CreateTreeNode(TreeNode^ parentTreeNode,Sophia::cSceneNode* pNode);
	private:
		// Returns the bounds of the specified node, including the region 
		// occupied by the node label and any node tag displayed.
		System::Drawing::Rectangle NodeBounds( TreeNode^ node );
	private:
		void DrawTreeNodeHighlightSelectedEvenWithoutFocus(System::Object^ sender, DrawTreeNodeEventArgs^ e);
	public: 
		System::Void listAnimation_SelectedIndexChanged( System::Object^ sender, System::EventArgs^ e );
	public:	
		System::Void checkBox_showPartialWeight_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	};
}
