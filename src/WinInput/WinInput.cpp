#include "stdafx.h"
#include "WinInput.h"

Input* cStaticWinInput::m_pWinInput=NULL;

cWinInput::cWinInput()
{
	//m_pWinInput=this;
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


Input::Input(void)
{
	m_pWinInput=this;
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
}

Input::~Input(void)
{
}


bool Input::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;


	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if(FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void Input::Shutdown()
{
	// Release the mouse.
	if(m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if(m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if(m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}


void Input::Update(DWORD elapseTime)
{
	// Read the current state of the keyboard.
	ReadKeyboard();
	// Read the current state of the mouse.
	ReadMouse();
	// Process the changes in the mouse and keyboard.
	ProcessInput();
}


bool Input::ReadKeyboard()
{
	HRESULT result;


	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if(FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


bool Input::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if(FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


void Input::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if(m_mouseX < 0)  { m_mouseX = 0; }
	if(m_mouseY < 0)  { m_mouseY = 0; }

	if(m_mouseX > m_screenWidth)  { m_mouseX = m_screenWidth; }
	if(m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	for (int i=0;i<4;i++)
	{
		m_mouseButtonTurnUp[i] = (m_mouseButtonPrev[i] ^ m_mouseState.rgbButtons[i]) & m_mouseButtonPrev[i];		
		m_mouseButtonTurnDn[i] = (m_mouseButtonPrev[i] ^ m_mouseState.rgbButtons[i]) & m_mouseState.rgbButtons[i];		
	}
	memcpy(&m_mouseButtonPrev,&m_mouseState.rgbButtons,4);	

	for (int i=0;i<256;i++)
	{
		m_keyboardStateTurnUp[i] = (m_keyboardStatePrev[i] ^ m_keyboardState[i]) & m_keyboardStatePrev[i];		
		m_keyboardStateTurnDn[i] = (m_keyboardStatePrev[i] ^ m_keyboardState[i]) & m_keyboardState[i];		
	}
	memcpy(&m_keyboardStatePrev,&m_keyboardState,256);	

	return;
}


bool Input::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}


void Input::GetMouseLocation(LONG& mouseX, LONG& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

void Input::GetMouseDelta( LONG& mouseX, LONG& mouseY )
{
	mouseX = m_mouseState.lX;
	mouseY = m_mouseState.lY;
}

bool Input::IsTurnDn(BYTE dik)
{
	if(m_keyboardStateTurnDn[dik]&0x80)
		return TRUE;

	return FALSE;
}

bool Input::IsTurnUp(BYTE dik)
{
	if(m_keyboardStateTurnUp[dik]&0x80)
		return TRUE;

	return FALSE;

}

bool Input::IsCurrDn(BYTE dik)
{
	if(m_keyboardState[dik]&0x80)
		return TRUE;

	return FALSE;
}

bool Input::IsCurrUp(BYTE dik)
{
	if(m_keyboardState[dik]&0x80)
		return FALSE;

	return TRUE;

}

bool Input::Mouse_IsTurnDn(BYTE button)
{
	if(m_keyboardStateTurnDn[button]&0x80)
		return TRUE;

	return FALSE;
}

bool Input::Mouse_IsTurnUp(BYTE button)
{
	if(m_keyboardStateTurnUp[button]&0x80)
		return TRUE;

	return FALSE;

}

bool Input::Mouse_IsCurrDn(BYTE button)
{
	if(m_mouseState.rgbButtons[button]&0x80)
		return TRUE;

	return FALSE;
}

bool Input::Mouse_IsCurrUp(BYTE button)
{
	if(m_mouseState.rgbButtons[button]&0x80)
		return FALSE;

	return TRUE;

}