#pragma once

#include "Foundation/Interface.h"
#include "Framework/StaticHWND.h"

#define VK_MAX 256		// 전체 가상 키의 수

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam)	((int)(short)HIWORD(lParam))
#endif

class cWinInput;

class cStaticWinInput
{
public:
	cStaticWinInput() {};
	~cStaticWinInput() {};
protected:
	static cWinInput* m_pWinInput;	

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
	BYTE	m_byVKPrev[VK_MAX];		// 이전 키의 정보
	BYTE	m_byVKCurr[VK_MAX];		// 현재 키의 정보
	BYTE	m_byVKTurnDn[VK_MAX];	// Down된 키의 정보
	BYTE	m_byVKTurnUp[VK_MAX];   // Up 된 키의 정보	
	POINT	m_ptMouseCurr;
	POINT	m_ptMousePrev;
	SIZE	m_MouseDelta;
	BOOL	m_bUseMsgProc;
	
	
public:
	// ::GetKeyboardState() 가 작동하지않을경우 MsgProc를 사용한다.
	void	MsgProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

	virtual void	Update(DWORD elapseTime);
	BOOL	IsTurnDn(BYTE vk);
	BOOL	IsTurnUp(BYTE vk);
	BOOL	IsCurrDn(BYTE vk);
	BOOL	IsCurrUp(BYTE vk);
	POINT&	GetMousePoint();
	SIZE&	GetMouseDelta();
	
};
