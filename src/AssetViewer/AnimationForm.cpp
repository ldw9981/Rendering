#include "StdAfx.h"
#include "AnimationForm.h"
#include "Graphics/Animation.h"
#include "State.h"

void AssetViewer::AnimationForm::Update( State* pState,Sophia::cSceneNode* pNode )
{
	m_pState = pState;
	m_pNode = pNode;

	if (m_pEntity != pState->GetEntity() || listAnimation->Items->Count != pState->GetEntity()->GetVecAnimation().size())
	{
		textCutName->Text = "";
		textLength->Text = Convert::ToString(0);
		textStartTime->Text = Convert::ToString(0);
		textEndTime->Text = Convert::ToString(0);

		listAnimation->Items->Clear();
		comboBoxBase->Items->Clear();
		comboBoxPartial->Items->Clear();				

		auto container = pState->GetEntity()->GetVecAnimation();
		for (size_t i = 0;i<container.size();i++)
		{
			System::String^ text = gcnew System::String(container[i]->GetUniqueKey().c_str());
			listAnimation->Items->Add(text);
			comboBoxBase->Items->Add(text);
			comboBoxPartial->Items->Add(text);
		}		

		if (comboBoxBase->SelectedIndex == -1 && !container.empty())
		{
			comboBoxBase->SelectedIndex = 0;
		}
	}

	textBox_PartialWeight->Text = L"";
	textBox_PartialWeight->Enabled = false;
	if (m_pNode && listAnimation->SelectedIndex != -1)
	{
		if( m_pNode->CountSceneAnimation() > 0)
		{
			Sophia::SceneAnimation* pSceneAnimation = m_pNode->GetSceneAnimation(listAnimation->SelectedIndex);
			if (pSceneAnimation)
			{
				textBox_PartialWeight->Text = Convert::ToString(pSceneAnimation->m_partialWeight);
				textBox_PartialWeight->Enabled = true;
			}	
		}
	}

	UpdateText(pState->GetModifiedAnimation());
	m_pEntity = pState->GetEntity();
}

System::Void AssetViewer::AnimationForm::listAnimation_OnSelectedIndexChanged( System::Object^ sender, System::EventArgs^ e )
{
	if (listAnimation->SelectedIndex == -1)
	{
		textLength->Text = L"";
		textStartTime->Text = L"";
		textEndTime->Text = L"";
	}
	else
	{
		auto container = m_pState->GetEntity()->GetVecAnimation();
		textLength->Text = Convert::ToString((int)container[listAnimation->SelectedIndex]->m_dwTimeLength);
		textStartTime->Text = Convert::ToString((int)0);
		textEndTime->Text = Convert::ToString((int)container[listAnimation->SelectedIndex]->m_dwTimeLength);
	}

}

System::Void AssetViewer::AnimationForm::OnShown( System::Object^ sender, System::EventArgs^ e )
{
	
}


System::Void AssetViewer::AnimationForm::loop_OnCheckedChanged( System::Object^ sender, System::EventArgs^ e )
{

}

System::Void AssetViewer::AnimationForm::textStartTime_KeyPress( System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e )
{
	if (!(System::Char::IsDigit(e->KeyChar) || e->KeyChar == System::Convert::ToChar(Keys::Back))) 
	{
		if (e->KeyChar == (char)System::Windows::Forms::Keys::Return)
		{
			int length = Convert::ToInt32(textLength->Text);
			int startTime = Convert::ToInt32(textStartTime->Text);
			int endTime = Convert::ToInt32(textEndTime->Text);

			if( startTime >= (endTime) )
			{
				textStartTime->Text = Convert::ToString(0);
			}
		}
		else
		{
			e->Handled = true; 
		}		
	}

}

void AssetViewer::AnimationForm::PlayAnimation()
{

}

System::Void AssetViewer::AnimationForm::textEndTime_KeyPress( System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e )
{
	if (!(System::Char::IsDigit(e->KeyChar) || e->KeyChar == System::Convert::ToChar(Keys::Back))) 
	{
		if (e->KeyChar == (char)System::Windows::Forms::Keys::Return)
		{
			MakeValidCutTime();
		}
		else
		{
			e->Handled = true; 
		}		
	}
}

System::Void AssetViewer::AnimationForm::buttonCut_Click( System::Object^ sender, System::EventArgs^ e )
{
	if (listAnimation->SelectedIndex == -1)
		return;

	if( textCutName->Text->Length == 0 )
		return;

	std::string suffix = msclr::interop::marshal_as< std::string >( textCutName->Text);
	std::string key = m_pState->GetEntity()->GetNodeName() + std::string("_") + suffix;

	if( listAnimation->Items->Contains(gcnew System::String(key.c_str())) )
	{
		MessageBox::Show(this,"An animation with this name is already exist.","Error");
		textCutName->Text = "";
		return;
	}
	
	int length = Convert::ToInt32(textLength->Text);
	int startTime = Convert::ToInt32(textStartTime->Text);
	int endTime = Convert::ToInt32(textEndTime->Text);

	m_pState->GetEntity()->CutAndPushEntityAnimation(listAnimation->SelectedIndex,startTime,endTime,suffix.c_str());
	m_pState->SetModifiedAnimation(true);
	Update(m_pState,m_pNode);
}

System::Void AssetViewer::AnimationForm::buttonDel_Click( System::Object^ sender, System::EventArgs^ e )
{
	if (listAnimation->SelectedIndex == -1)
		return;

	if (comboBoxBase->SelectedIndex == listAnimation->SelectedIndex )
	{
		m_pState->GetEntity()->StopBaseAnimation();
		comboBoxBase->SelectedIndex = -1;
	}

	if (comboBoxPartial->SelectedIndex == listAnimation->SelectedIndex )
	{
		m_pState->GetEntity()->StopPartialAnimation(listAnimation->SelectedIndex);
		comboBoxPartial->SelectedIndex = -1;
	}

	m_pState->GetEntity()->EraseAnimation(listAnimation->SelectedIndex);
	m_pState->SetModifiedAnimation(true);

	Update(m_pState,m_pNode);
}

void AssetViewer::AnimationForm::UpdateText( bool modified )
{
	if (modified)
	{
		this->Text = L"Animation*";
	}
	else
	{
		this->Text = L"Animation";
	}
}

System::Void AssetViewer::AnimationForm::textBox_PartialWeight_KeyDown( System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e )
{
	if (e->KeyCode != Keys::Enter)
		return;

	float weight = (float)Convert::ToDouble(textBox_PartialWeight->Text);
	weight = max(weight,0.0f);
	weight = min(weight,1.0f);
	textBox_PartialWeight->Text = Convert::ToString(weight);

	if (listAnimation->SelectedIndex == -1)
		return;
	
	if ( m_pNode == NULL )
		return;

	if (m_pNode->CountSceneAnimation() == 0)
		return;

	Sophia::SceneAnimation* pSceneAnimation = m_pNode->GetSceneAnimation(listAnimation->SelectedIndex);
	if (!pSceneAnimation)
		return;

	pSceneAnimation->m_partialWeight = weight;
	m_pState->SetModifiedAnimation(true);
	UpdateText(true);
	this->ActiveControl = splitContainer1->Panel1;
	e->SuppressKeyPress = true;
}

System::Void AssetViewer::AnimationForm::textBox_PartialWeight_KeyPress( System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e )
{

}

System::Void AssetViewer::AnimationForm::button_Base_Click( System::Object^ sender, System::EventArgs^ e )
{
	if (comboBoxBase->SelectedIndex == -1)
		return;

	int length = Convert::ToInt32(textLength->Text);
	int startTime = Convert::ToInt32(textStartTime->Text);
	int endTime = Convert::ToInt32(textEndTime->Text);

	m_pState->GetEntity()->PlayBaseAnimation(comboBoxBase->SelectedIndex,
		checkBox_baseLoop->Checked,startTime,length-endTime);
}

System::Void AssetViewer::AnimationForm::button_Partial_Click( System::Object^ sender, System::EventArgs^ e )
{
	if (comboBoxPartial->SelectedIndex == -1)
		return;

	int length = Convert::ToInt32(textLength->Text);
	int startTime = Convert::ToInt32(textStartTime->Text);
	int endTime = Convert::ToInt32(textEndTime->Text);

	m_pState->GetEntity()->PlayPartialAnimation(comboBoxPartial->SelectedIndex,
		checkBox_partialLoop->Checked,startTime,length-endTime);
}

System::Void AssetViewer::AnimationForm::button_BaseStop_Click( System::Object^ sender, System::EventArgs^ e )
{
	if (m_pState && m_pEntity)
	{
		m_pEntity->StopBaseAnimation();
	}
}

System::Void AssetViewer::AnimationForm::button_PartialStop_Click( System::Object^ sender, System::EventArgs^ e )
{
	if (comboBoxPartial->SelectedIndex == -1)
		return;

	if (m_pState && m_pEntity)
	{
		m_pEntity->StopPartialAnimation(comboBoxPartial->SelectedIndex);
	}
}

void AssetViewer::AnimationForm::Clear()
{
	m_pState = NULL;
	m_pNode = NULL;
	m_pEntity = NULL;
	textCutName->Text = "";
	textLength->Text = Convert::ToString(0);
	textStartTime->Text = Convert::ToString(0);
	textEndTime->Text = Convert::ToString(0);

	listAnimation->Items->Clear();
	comboBoxBase->Items->Clear();
	comboBoxPartial->Items->Clear();

	textBox_PartialWeight->Text = L"";
	textBox_PartialWeight->Enabled = false;
}

System::Void AssetViewer::AnimationForm::textEndTime_Leave( System::Object^ sender, System::EventArgs^ e )
{
	MakeValidCutTime();
}

void AssetViewer::AnimationForm::MakeValidCutTime()
{
	int length = Convert::ToInt32(textLength->Text);
	int startTime = Convert::ToInt32(textStartTime->Text);
	int endTime = Convert::ToInt32(textEndTime->Text);

	if(startTime>length) 
	{
		textStartTime->Text = Convert::ToString(0);
	}	

	if( (endTime>length) || (endTime < startTime))
	{
		textEndTime->Text = Convert::ToString(length);
	}	

}

System::Void AssetViewer::AnimationForm::textStartTime_Leave( System::Object^ sender, System::EventArgs^ e )
{
	MakeValidCutTime();
}



