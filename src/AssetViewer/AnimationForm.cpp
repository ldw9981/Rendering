#include "StdAfx.h"
#include "AnimationForm.h"
#include "Graphics/Animation.h"
#include "State.h"

void AssetViewer::AnimationForm::Update( State* pState )
{
	m_pState = pState;

	textCutName->Text = "";
	textLength->Text = Convert::ToString(0);
	textStartTime->Text = Convert::ToString(0);
	textEndTime->Text = Convert::ToString(0);
	listAnimation->Items->Clear();
	m_selectedIndexLast = listAnimation->SelectedIndex;
	auto container = pState->GetEntity()->GetVecAnimation();
	for (size_t i = 0;i<container.size();i++)
	{
		System::String^ text = gcnew System::String(container[i]->GetUniqueKey().c_str());
		listAnimation->Items->Add(text);
	}
	UpdateText(m_pState->GetModifiedAnimation());
}

System::Void AssetViewer::AnimationForm::listAnimation_OnSelectedIndexChanged( System::Object^ sender, System::EventArgs^ e )
{
	if (listAnimation->SelectedIndex == -1)
		return;

	if (m_selectedIndexLast != listAnimation->SelectedIndex)
	{
		auto container = m_pState->GetEntity()->GetVecAnimation();
		textLength->Text = Convert::ToString((int)container[listAnimation->SelectedIndex]->m_dwTimeLength);
		textStartTime->Text = Convert::ToString((int)0);
		textEndTime->Text = Convert::ToString((int)container[listAnimation->SelectedIndex]->m_dwTimeLength);
	}

	PlayAnimation();
	m_selectedIndexLast = listAnimation->SelectedIndex;
}

System::Void AssetViewer::AnimationForm::OnShown( System::Object^ sender, System::EventArgs^ e )
{
	
}


System::Void AssetViewer::AnimationForm::loop_OnCheckedChanged( System::Object^ sender, System::EventArgs^ e )
{

	PlayAnimation();
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
			else
			{
				PlayAnimation();
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
	if (listAnimation->SelectedIndex == -1)
		return;

	int length = Convert::ToInt32(textLength->Text);
	int startTime = Convert::ToInt32(textStartTime->Text);
	int endTime = Convert::ToInt32(textEndTime->Text);

	m_pState->GetEntity()->PlayAnimation(listAnimation->SelectedIndex,
		checkBox1->Checked,startTime,length-endTime);
}

System::Void AssetViewer::AnimationForm::textEndTime_KeyPress( System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e )
{
	if (!(System::Char::IsDigit(e->KeyChar) || e->KeyChar == System::Convert::ToChar(Keys::Back))) 
	{
		if (e->KeyChar == (char)System::Windows::Forms::Keys::Return)
		{
			int length = Convert::ToInt32(textLength->Text);
			int startTime = Convert::ToInt32(textStartTime->Text);
			int endTime = Convert::ToInt32(textEndTime->Text);

			if( (endTime>length) || (endTime < startTime))
			{
				textEndTime->Text = Convert::ToString(length);
			}		
			else
			{
				PlayAnimation();
			}			
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
	int startTime = Convert::ToInt32(textStartTime->Text);
	int endTime = Convert::ToInt32(textEndTime->Text);
	m_pState->GetEntity()->CutAndPushEntityAnimation(listAnimation->SelectedIndex,startTime,endTime,suffix.c_str());
	m_pState->SetModifiedAnimation(true);
	Update(m_pState);
}

System::Void AssetViewer::AnimationForm::buttonDel_Click( System::Object^ sender, System::EventArgs^ e )
{
	if (listAnimation->SelectedIndex == -1)
		return;

	m_pState->GetEntity()->StopAnimation();
	m_pState->GetEntity()->EraseAnimation(listAnimation->SelectedIndex);
	m_pState->SetModifiedAnimation(true);
	Update(m_pState);
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
