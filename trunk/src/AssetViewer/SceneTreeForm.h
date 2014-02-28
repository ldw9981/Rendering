#pragma once
#include "Framework.h"
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
			
			imageList.Images->Add(Bitmap::FromFile("scenetree_root.png"));
			imageList.Images->Add(Bitmap::FromFile("scenetree_scene.png"));
			imageList.Images->Add(Bitmap::FromFile("scenetree_mesh.png"));
			imageList.Images->Add(Bitmap::FromFile("scenetree_skin.png"));
			imageList.Images->Add(Bitmap::FromFile("scenetree_skeleton.png"));

			treeView1->ImageList = %imageList;
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
		}

	public:
	
	protected: 
		State*			m_pState;
		ImageList imageList;
	private:
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>
		System::ComponentModel::Container ^components;
		System::Windows::Forms::TreeView^  treeView1;


			 
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
			this->treeView1->Location = System::Drawing::Point(0, 0);
			this->treeView1->Name = L"treeView1";
			this->treeView1->Size = System::Drawing::Size(240, 474);
			this->treeView1->TabIndex = 0;
			this->treeView1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &SceneTreeForm::treeview_OnMouseClick);
			// 
			// SceneTreeForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(240, 474);
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
		void Update(State* pState)
		{
			m_pState = pState;
			treeView1->Nodes->Clear();
			TreeNode^ svrNode = CreateTreeNode(nullptr,pState->GetEntity());
			treeView1->Nodes->Add(svrNode);
			// 모든 트리 노드를 보여준다
			treeView1->ExpandAll();		
		
		}
	public: 
		TreeNode^ CreateTreeNode(TreeNode^ parentTreeNode,Sophia::cSceneNode* pNode)
		{
			System::String^ name = gcnew System::String(pNode->GetNodeName().c_str());
			TreeNode^ treeNode;
		
			int img = pNode->GetType();
			if (parentTreeNode == nullptr)
			{
				treeNode = gcnew TreeNode(name, img, img);			
			}
			else
			{				
				if (pNode->GetIsBone())
				{
					img = 4;
				}
				treeNode = parentTreeNode->Nodes->Add( name , name,img,img);
			}
			
			for ( auto it = pNode->m_listChildNode.begin() ; it != pNode->m_listChildNode.end() ; it++ )
			{
				CreateTreeNode(treeNode,*it);
			}
			return treeNode;
		}
	
	};
}
