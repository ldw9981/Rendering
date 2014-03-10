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
		ImageList imageList;
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
	private:
		// Returns the bounds of the specified node, including the region 
		// occupied by the node label and any node tag displayed.
		System::Drawing::Rectangle NodeBounds( TreeNode^ node )
		{
			// Set the return value to the normal node bounds.
			System::Drawing::Rectangle bounds = node->Bounds;
			if ( node->Tag != nullptr )
			{
				// Retrieve a Graphics object from the TreeView handle
				// and use it to calculate the display width of the tag.
				Graphics^ g = treeView1->CreateGraphics();
				int tagWidth = (int)g->MeasureString( node->Tag->ToString(), tagFont ).Width + 6;

				// Adjust the node bounds using the calculated value.
				bounds.Offset( tagWidth / 2, 0 );
				bounds = Rectangle::Inflate( bounds, tagWidth / 2, 0 );
				g->~Graphics();
			}

			return bounds;
		}
	private:
		void DrawTreeNodeHighlightSelectedEvenWithoutFocus(System::Object^ sender, DrawTreeNodeEventArgs^ e)
		{	
			// Draw the background and node text for a selected node.
			if ( (e->State & TreeNodeStates::Selected) != (TreeNodeStates)0 )
			{
				// Draw the background of the selected node. The NodeBounds
				// method makes the highlight rectangle large enough to
				// include the text of a node tag, if one is present.
				e->Graphics->FillRectangle( Brushes::Blue,  NodeBounds( e->Node ) );

				// Retrieve the node font. If the node font has not been set,
				// use the TreeView font.
				System::Drawing::Font^ nodeFont = e->Node->NodeFont;
				if ( nodeFont == nullptr )
					nodeFont = (dynamic_cast<TreeView^>(sender))->Font;

				// Draw the node text.
				e->Graphics->DrawString( e->Node->Text, nodeFont, Brushes::White, Rectangle::Inflate( e->Bounds, 2, 0 ) );
			}
			// Use the default background and node text.
			else
			{
				e->DrawDefault = true;
			}

			// If a node tag is present, draw its string representation 
			// to the right of the label text.
			if ( e->Node->Tag != nullptr )
			{
				e->Graphics->DrawString( e->Node->Tag->ToString(), tagFont, Brushes::Yellow, (float)e->Bounds.Right + 2, (float)e->Bounds.Top );
			}


			// If the node has focus, draw the focus rectangle large, making
			// it large enough to include the text of the node tag, if present.
			if ( (e->State & TreeNodeStates::Focused) != (TreeNodeStates)0 )
			{
				Pen^ focusPen = gcnew Pen( Color::Black );
				try
				{
					focusPen->DashStyle = System::Drawing::Drawing2D::DashStyle::Dot;
					System::Drawing::Rectangle focusBounds = NodeBounds( e->Node );
					focusBounds.Size = System::Drawing::Size( focusBounds.Width - 1, focusBounds.Height - 1 );
					e->Graphics->DrawRectangle( focusPen, focusBounds );
				}
				finally
				{
					if ( focusPen )
						delete safe_cast<IDisposable^>(focusPen);
				}

			}

		}

	};
}
