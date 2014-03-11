#include "StdAfx.h"
#include "SceneTreeForm.h"


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

void AssetViewer::SceneTreeForm::DrawTreeNodeHighlightSelectedEvenWithoutFocus( System::Object^ sender,DrawTreeNodeEventArgs^ e )
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

System::Drawing::Rectangle AssetViewer::SceneTreeForm::NodeBounds( TreeNode^ node )
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

TreeNode^ AssetViewer::SceneTreeForm::CreateTreeNode( TreeNode^ parentTreeNode,Sophia::cSceneNode* pNode )
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
