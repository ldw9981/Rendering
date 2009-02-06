#include "stdafx.h"
#include "WinInput.h"

cWinInput* cStaticWinInput::m_pWinInput=NULL;

cWinInput::cWinInput()
{
	m_pWinInput=this;
	m_bUseMsgProc=FALSE;
	memset(&m_byVKCurr,0,VK_MAX);
	memset(&m_byVKPrev,0,VK_MAX);
	memset(&m_byVKTurnDn,0,VK_MAX);
	memset(&m_byVKTurnUp,0,VK_MAX);
	memset(&m_ptMouseCurr,0,sizeof(m_ptMouseCurr));
}

cWinInput::~cWinInput(void)
{
}

void cWinInput::Update(DWORD elapseTime)
{
	int i=0;
	if (!m_bUseMsgProc)
	{
		::GetKeyboardState((PBYTE)&m_byVKCurr);

		::GetCursorPos(&m_ptMouseCurr);
		if (m_hWnd!=NULL)
		{
			::ScreenToClient(m_hWnd,&m_ptMouseCurr);
		}		
		m_MouseDelta.cx = m_ptMousePrev.x - m_ptMouseCurr.x;
		m_MouseDelta.cy = m_ptMousePrev.y - m_ptMouseCurr.y;

	}
	
	for (i=0;i<VK_MAX;i++)
	{
		m_byVKTurnUp[i] = (m_byVKPrev[i] ^ m_byVKCurr[i]) & m_byVKPrev[i];		
		m_byVKTurnDn[i] = (m_byVKPrev[i] ^ m_byVKCurr[i]) & m_byVKCurr[i];		
	}
	memcpy(&m_byVKPrev,&m_byVKCurr,VK_MAX);	
	m_ptMousePrev=m_ptMouseCurr;

}


BOOL cWinInput::IsTurnDn(BYTE vk)
{
	if(m_byVKTurnDn[vk]&0x80)
		return TRUE;

	return FALSE;
}

BOOL cWinInput::IsTurnUp(BYTE vk)
{
	if(m_byVKTurnUp[vk]&0x80)
		return TRUE;

	return FALSE;

}

BOOL cWinInput::IsCurrDn(BYTE vk)
{
	if(m_byVKCurr[vk]&0x80)
		return TRUE;

	return FALSE;
}

BOOL cWinInput::IsCurrUp(BYTE vk)
{
	if(m_byVKCurr[vk]&0x80)
		return FALSE;

	return TRUE;

}

void cWinInput::MsgProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	assert(m_bUseMsgProc);
	switch(message)
	{
	case WM_MOUSEMOVE:
		m_ptMouseCurr.x= GET_X_LPARAM(lParam); 
		m_ptMouseCurr.y= GET_Y_LPARAM(lParam); 		
		break;
	case WM_KEYDOWN:
		m_byVKCurr[wParam]=0x80;
		break;
	case WM_KEYUP:
		m_byVKCurr[wParam]=0x0;
		break;	
	case WM_LBUTTONDOWN:
		m_byVKCurr[VK_LBUTTON]=0x80;
		break;
	case WM_LBUTTONUP:
		m_byVKCurr[VK_LBUTTON]=0x0;
		break;
	case WM_RBUTTONDOWN:
		m_byVKCurr[VK_RBUTTON]=0x80;
		break;
	case WM_RBUTTONUP:
		m_byVKCurr[VK_RBUTTON]=0x0;
		break;
	}
}
POINT& cWinInput::GetMousePoint()
{
	return m_ptMouseCurr;
}

SIZE& cWinInput::GetMouseDelta()
{
	return m_MouseDelta;
}	
