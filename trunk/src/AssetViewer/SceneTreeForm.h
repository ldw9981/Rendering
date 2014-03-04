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
	/// SceneTreeForm�� ���� ����Դϴ�.
	/// </summary>
	public ref class SceneTreeForm : public System::Windows::Forms::Form
	{
	public:
		SceneTreeForm(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//
					
			Reflection::Assembly^ pxAssembly = Reflection::Assembly::GetExecutingAssembly();
			String^ pxResName = pxAssembly->GetName()->Name + ".SceneTreeForm"; //Note: add your resourcefile name here, i.e. ".MyResourceFile" as it appears in solution explorer, without it's extension
			auto s_pxResourceManager = (gcnew Resources::ResourceManager(pxResName,pxAssembly));

			//pictureBox1->Image =  //note: this is the exact name from the resx, not the filename or name in solution explorer. By default it's name in the resx won't contain it's extension.

			imageList.Images->Add((cli::safe_cast<Drawing::Bitmap^>(s_pxResourceManager->GetObject("scenetree_root"))));
			imageList.Images->Add((cli::safe_cast<Drawing::Bitmap^>(s_pxResourceManager->GetObject("scenetree_scene"))));
			imageList.Images->Add((cli::safe_cast<Drawing::Bitmap^>(s_pxResourceManager->GetObject("scenetree_mesh"))));
			imageList.Images->Add((cli::safe_cast<Drawing::Bitmap^>(s_pxResourceManager->GetObject("scenetree_skin"))));
			imageList.Images->Add((cli::safe_cast<Drawing::Bitmap^>(s_pxResourceManager->GetObject("scenetree_skeleton"))));
			treeView1->ImageList = %imageList;			
		}

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
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
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>
		System::ComponentModel::Container ^components;
	public:
		System::Windows::Forms::TreeView^  treeView1;


			 
#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		void InitializeComponent(void)
		{
			this->treeView1 = (gcnew System::Windows::Forms::TreeView());
			this->SuspendLayout();
			// 
			// treeView1
			// 
			this->treeView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->treeView1->HideSelection = false;
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
			// ��� Ʈ�� ��带 �����ش�
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
