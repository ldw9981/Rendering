#include "StdAfx.h"
#include "CameraSettingForm.h"
#include "State.h"



void AssetViewer::CameraSettingForm::Update( State* pState )
{
	m_pState = pState;

	const D3DXVECTOR3* pos = m_pState->m_graphicWorld.m_camera.GetLocalPos();
	textBoxCamPosX->Text = Convert::ToString(pos->x);
	textBoxCamPosY->Text = Convert::ToString(pos->y);
	textBoxCamPosZ->Text = Convert::ToString(pos->z);
	textBoxZScrollSize->Text = Convert::ToString(m_pState->m_scrollSize);

}



System::Void AssetViewer::CameraSettingForm::textBoxCamPosX_KeyPress( System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e )
{
	

}

System::Void AssetViewer::CameraSettingForm::textBoxCamPosX_KeyDown( System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e )
{
	if (e->KeyCode != Keys::Enter)
		return;

	D3DXVECTOR3 pos;
	pos.x = (float)Convert::ToDouble(textBoxCamPosX->Text);
	pos.y = (float)Convert::ToDouble(textBoxCamPosY->Text);
	pos.z = (float)Convert::ToDouble(textBoxCamPosZ->Text);
	
	m_pState->m_graphicWorld.m_camera.SetLocalPos(pos);
	e->SuppressKeyPress = true;
}

System::Void AssetViewer::CameraSettingForm::textBoxCamPosY_KeyDown( System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e )
{
	if (e->KeyCode != Keys::Enter)
		return;

	D3DXVECTOR3 pos;
	pos.x = (float)Convert::ToDouble(textBoxCamPosX->Text);
	pos.y = (float)Convert::ToDouble(textBoxCamPosY->Text);
	pos.z = (float)Convert::ToDouble(textBoxCamPosZ->Text);

	m_pState->m_graphicWorld.m_camera.SetLocalPos(pos);
	e->SuppressKeyPress = true;
}

System::Void AssetViewer::CameraSettingForm::textBoxCamPosZ_KeyDown( System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e )
{
	if (e->KeyCode != Keys::Enter)
		return;

	D3DXVECTOR3 pos;
	pos.x = (float)Convert::ToDouble(textBoxCamPosX->Text);
	pos.y = (float)Convert::ToDouble(textBoxCamPosY->Text);
	pos.z = (float)Convert::ToDouble(textBoxCamPosZ->Text);

	m_pState->m_graphicWorld.m_camera.SetLocalPos(pos);
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
