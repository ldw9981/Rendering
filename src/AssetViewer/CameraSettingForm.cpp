#include "StdAfx.h"
#include "CameraSettingForm.h"
#include "State.h"



void AssetViewer::CameraSettingForm::UpdateForm( State* pState )
{
	m_pState = pState;

	const D3DXVECTOR3* pos = m_pState->m_graphicWorld.m_camera.GetLocalPos();
	textBoxCamPosX->Text = Convert::ToString(pos->x);
	textBoxCamPosY->Text = Convert::ToString(pos->y);
	textBoxCamPosZ->Text = Convert::ToString(pos->z);
	textBoxZScrollSize->Text = Convert::ToString(m_pState->m_scrollSize);

	const D3DXVECTOR3* pLookAt = m_pState->m_graphicWorld.m_camera.GetForward();
	textBoxLookAtPosX->Text = Convert::ToString(pLookAt->x);
	textBoxLookAtPosY->Text = Convert::ToString(pLookAt->y);
	textBoxLookAtPosZ->Text = Convert::ToString(pLookAt->z);
	
	const D3DXVECTOR3* pUp = m_pState->m_graphicWorld.m_camera.GetUp();
	textBoxUpVectorX->Text = Convert::ToString(pUp->x);
	textBoxUpVectorY->Text = Convert::ToString(pUp->y);
	textBoxUpVectorZ->Text = Convert::ToString(pUp->z);
	

	const D3DXVECTOR3& dir = m_pState->m_graphicWorld.GetWorldLightDirection();
	textDirectionLightX->Text = Convert::ToString(dir.x);
	textDirectionLightY->Text = Convert::ToString(dir.y);
	textDirectionLightZ->Text = Convert::ToString(dir.z);

}




System::Void AssetViewer::CameraSettingForm::textBoxCamPos_KeyDown( System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e )
{
	if (e->KeyCode != Keys::Enter)
		return;
	
	UpdateCamera();	
	
	e->SuppressKeyPress = true;
}

System::Void AssetViewer::CameraSettingForm::textBoxZScrollSize_KeyDown( System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e )
{
	if (e->KeyCode != Keys::Enter)
		return;

	float data = (float)Convert::ToDouble(textBoxZScrollSize->Text);

	m_pState->m_scrollSize = data;
	e->SuppressKeyPress = true;
}

System::Void AssetViewer::CameraSettingForm::textBoxCamPos_Leave( System::Object^ sender, System::EventArgs^ e )
{
	UpdateCamera();	
}

System::Void AssetViewer::CameraSettingForm::textDirectionLight_Leave( System::Object^ sender, System::EventArgs^ e )
{
	D3DXVECTOR3 pos;
	pos.x = (float)Convert::ToDouble(textBoxCamPosX->Text);
	pos.y = (float)Convert::ToDouble(textBoxCamPosY->Text);
	pos.z = (float)Convert::ToDouble(textBoxCamPosZ->Text);

	m_pState->m_graphicWorld.SetWorldLightDirection(pos);
}

System::Void AssetViewer::CameraSettingForm::textDirectionLight_KeyDown( System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e )
{
	D3DXVECTOR3 pos;
	pos.x = (float)Convert::ToDouble(textBoxCamPosX->Text);
	pos.y = (float)Convert::ToDouble(textBoxCamPosY->Text);
	pos.z = (float)Convert::ToDouble(textBoxCamPosZ->Text);

	m_pState->m_graphicWorld.SetWorldLightDirection(pos);
}

System::Void AssetViewer::CameraSettingForm::textBoxLookAtPos_Leave( System::Object^ sender, System::EventArgs^ e )
{
	UpdateCamera();	
}

System::Void AssetViewer::CameraSettingForm::textBoxLookAtPos_KeyDown( System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e )
{
	UpdateCamera();	
}

void AssetViewer::CameraSettingForm::UpdateCamera()
{
	D3DXVECTOR3 pos;
	pos.x = (float)Convert::ToDouble(textBoxCamPosX->Text);
	pos.y = (float)Convert::ToDouble(textBoxCamPosY->Text);
	pos.z = (float)Convert::ToDouble(textBoxCamPosZ->Text);

	D3DXVECTOR3 at;
	at.x = (float)Convert::ToDouble(textBoxLookAtPosX->Text);
	at.y = (float)Convert::ToDouble(textBoxLookAtPosY->Text);
	at.z = (float)Convert::ToDouble(textBoxLookAtPosZ->Text);

	D3DXVECTOR3 up;
	up.x = (float)Convert::ToDouble(textBoxUpVectorX->Text);
	up.y = (float)Convert::ToDouble(textBoxUpVectorY->Text);
	up.z = (float)Convert::ToDouble(textBoxUpVectorZ->Text);

	m_pState->m_graphicWorld.m_camera.SetLookAt(&pos,&at,&up);
}

