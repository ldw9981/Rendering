#pragma once

#include "Foundation/Interface.h"
#include "Framework/StaticHWND.h"

#define VK_MAX 256		// ��ü ���� Ű�� ��

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam)	((int)(short)HIWORD(lParam))
#endif

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//////////////
// INCLUDES //
//////////////
#include <dinput.h>
class Input;
class cStaticWinInput
{
public:
	cStaticWinInput() {};
	~cStaticWinInput() {};
protected:
	static Input* m_pWinInput;	

};

class Input:
	public IUpdatable,
	public IUnknownObject,
	private cStaticWinInput
{
public:
	Input(void);
	virtual ~Input(void);

	bool Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	virtual void Update(DWORD elapseTime);

	bool IsEscapePressed();
	void GetMouseLocation(LONG& mouseX, LONG& mouseY);
	void GetMouseDelta(LONG& mouseX, LONG& mouseY);
	bool	IsTurnDn(BYTE diKeyboard);
	bool	IsTurnUp(BYTE diKeyboard);
	bool	IsCurrDn(BYTE diKeyboard);
	bool	IsCurrUp(BYTE diKeyboard);

	bool	Mouse_IsTurnDn(BYTE button);	//DIMOUSE
	bool	Mouse_IsTurnUp(BYTE button);
	bool	Mouse_IsCurrDn(BYTE button);
	bool	Mouse_IsCurrUp(BYTE button);
private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();


private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardStatePrev[256];
	unsigned char m_keyboardState[256];
	unsigned char m_keyboardStateTurnDn[256];
	unsigned char m_keyboardStateTurnUp[256];
	DIMOUSESTATE m_mouseState;
	BYTE m_mouseButtonPrev[4];
	BYTE m_mouseButtonTurnDn[4];
	BYTE m_mouseButtonTurnUp[4];

	LONG m_screenWidth, m_screenHeight;
	LONG m_mouseX, m_mouseY;
};





class cWinInput :
	private cStaticWinInput,
	public IUpdatable,
	public IUnknownObject,
	public StaticHWND
{
public:
	cWinInput();
	~cWinInput(void);	
protected:
	BYTE	m_byVKPrev[VK_MAX];		// ���� Ű�� ����
	BYTE	m_byVKCurr[VK_MAX];		// ���� Ű�� ����
	BYTE	m_byVKTurnDn[VK_MAX];	// Down�� Ű�� ����
	BYTE	m_byVKTurnUp[VK_MAX];   // Up �� Ű�� ����	
	POINT	m_ptMouseCurr;
	POINT	m_ptMousePrev;
	SIZE	m_MouseDelta;
	BOOL	m_bUseMsgProc;
	
	
public:
	// ::GetKeyboardState() �� �۵������������ MsgProc�� ����Ѵ�.
	void	MsgProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

	virtual void	Update(DWORD elapseTime);
	BOOL	IsTurnDn(BYTE vk);
	BOOL	IsTurnUp(BYTE vk);
	BOOL	IsCurrDn(BYTE vk);
	BOOL	IsCurrUp(BYTE vk);
	POINT&	GetMousePoint();
	SIZE&	GetMouseDelta();
	
};
