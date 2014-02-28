#include "StdAfx.h"
#include "AnimationForm.h"
#include "Graphics/Animation.h"
#include "State.h"

void AssetViewer::AnimationForm::Update( State* pState )
{
	m_pState = pState;

	
	textLength->Text = Convert::ToString(0);
	textSkipStartTime->Text = Convert::ToString(0);
	textEarlyEndTime->Text = Convert::ToString(0);
	listAnimation->Items->Clear();
	m_selectedIndexLast = listAnimation->SelectedIndex;
	auto container = pState->GetEntity()->GetVecAnimation();
	for (size_t i = 0;i<container.size();i++)
	{
		System::String^ text = gcnew System::String(container[i]->GetUniqueKey().c_str());
		listAnimation->Items->Add(text);
	}

}

System::Void AssetViewer::AnimationForm::listAnimation_OnSelectedIndexChanged( System::Object^ sender, System::EventArgs^ e )
{
	if (listAnimation->SelectedIndex == -1)
		return;

	if (m_selectedIndexLast != listAnimation->SelectedIndex)
	{
		auto container = m_pState->GetEntity()->GetVecAnimation();
		textLength->Text = Convert::ToString((int)container[listAnimation->SelectedIndex]->m_dwTimeLength);
		textSkipStartTime->Text = Convert::ToString((int)0);
		textEarlyEndTime->Text = Convert::ToString((int)0);
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

System::Void AssetViewer::AnimationForm::textSkipStartTime_KeyPress( System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e )
{
	if (!(System::Char::IsDigit(e->KeyChar) || e->KeyChar == System::Convert::ToChar(Keys::Back))) 
	{
		if (e->KeyChar == (char)System::Windows::Forms::Keys::Return)
		{
			int length = Convert::ToInt32(textLength->Text);
			int skipStartTime = Convert::ToInt32(textSkipStartTime->Text);
			int earlyEndTime = Convert::ToInt32(textEarlyEndTime->Text);

			if( skipStartTime > (length - earlyEndTime) )
			{
				textSkipStartTime->Text = Convert::ToString((int)(length - earlyEndTime));
			}

			PlayAnimation();
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

	m_pState->GetEntity()->PlayAnimation(listAnimation->SelectedIndex,checkBox1->Checked,Convert::ToInt32(textSkipStartTime->Text),Convert::ToInt32(textEarlyEndTime->Text));
}

System::Void AssetViewer::AnimationForm::textEarlyEndTime_KeyPress( System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e )
{
	if (!(System::Char::IsDigit(e->KeyChar) || e->KeyChar == System::Convert::ToChar(Keys::Back))) 
	{
		if (e->KeyChar == (char)System::Windows::Forms::Keys::Return)
		{
			int length = Convert::ToInt32(textLength->Text);
			int skipStartTime = Convert::ToInt32(textSkipStartTime->Text);
			int earlyEndTime = Convert::ToInt32(textEarlyEndTime->Text);

			if( earlyEndTime > (length - skipStartTime) )
			{
				textEarlyEndTime->Text = Convert::ToString((int)(length - skipStartTime));
			}

			PlayAnimation();
		}
		else
		{
			e->Handled = true; 
		}		
	}
}
