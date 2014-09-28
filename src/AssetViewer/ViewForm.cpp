#include "StdAfx.h"
#include "ViewForm.h"


System::Void AssetViewer::ViewForm::ViewForm_MouseClick( System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e )
{
	if (e->Button != System::Windows::Forms::MouseButtons::Right)
		return;
	
	System::Drawing::Point pt = this->PointToScreen(System::Drawing::Point(0,0));
	pt.X += e->Location.X;
	pt.Y += e->Location.Y;

	contextMenuStrip1->Show(pt);
}

System::Void AssetViewer::ViewForm::resetTransformToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
{
	if (!m_pState) 
		return;

	if (!m_pState->GetEntity())
		return;

	D3DXMATRIX temp;
	D3DXMatrixIdentity(&m_pState->m_helper.GetLocalTM());	
}

System::Void AssetViewer::ViewForm::showHideShadowToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
{
	if (!m_pState)
		return;

	m_pState->m_graphicWorld.SetEnableShadow(!m_pState->m_graphicWorld.GetEnableShadow());		
}
