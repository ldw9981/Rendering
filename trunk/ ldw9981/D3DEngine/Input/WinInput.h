#pragma once

#include "../Common/Interface.h"
#include "../Framework/StaticHWND.h"

#define VK_MAX 256		// ��ü ���� Ű�� ��

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
	public cIUpdatable,
	public IUnknownObject,
	public cStaticHWND
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
