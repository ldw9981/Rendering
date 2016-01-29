#include "StdAfx.h"
#include "ViewForm.h"
#include "MainForm.h"

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

System::Void AssetViewer::ViewForm::cameraSettingsToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
{
	if (cameraSettingForm != nullptr)
		return;

	cameraSettingForm = gcnew CameraSettingForm;
	cameraSettingForm->Owner = this->MdiParent;	
	cameraSettingForm->Closed += gcnew System::EventHandler(this, &ViewForm::cameraSettingsForm_Closed);
	cameraSettingForm->Show();
	cameraSettingForm->UpdateForm(m_pState);
}

System::Void AssetViewer::ViewForm::cameraSettingsForm_Closed( System::Object^ sender, System::EventArgs^ e )
{
	delete cameraSettingForm;
	cameraSettingForm = nullptr;
}

System::Void AssetViewer::ViewForm::OnApplicationIdle( System::Object^ sender,System::EventArgs^ e )
{
	while (AppStillIdle())
	{
		m_pFramework->Run();
	}			

	if (cameraSettingForm!=nullptr)
	{
		if (m_pState != NULL && this->Focused)
		{
			cameraSettingForm->UpdateForm(m_pState);
		}
	}
}
