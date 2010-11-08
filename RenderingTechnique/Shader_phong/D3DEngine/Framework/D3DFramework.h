#pragma once
#include "StaticHWND.h"

#include "framework.h"
#include "../Renderer/D3DGraphics.h"

class cD3DFramework;

interface	IUnknownObject;
class		cIRenderable;
class		cIProgressable;
interface	IControlable;




class cWinInput;
class cD3DGraphics;
class cD3DFramework :
	public cFramework,
	public cStaticD3DDEVICE9,
	public cStaticHWND
{
public:
	cD3DFramework(LPCWSTR szTitleName,BOOL m_bFullScreen,int nWidth,int nHeight);
	virtual ~cD3DFramework(void);

public:

	HINSTANCE				m_hInstance;		 	 
private:	
	
	WNDCLASS				m_wndclass;			// 메인 윈도우 클래스
	LPCTSTR					m_szClassName;		// 윈도우 클래스 등록 이름
	LPCTSTR					m_szTitleName;		// 윈도우 타이틀 이름
	RECT					m_rtClient;
	

	BOOL					m_bFullScreen;

	BOOL					m_bQuitLoop;
	list<cIRenderable*>		m_listRenderable;
	list<cIProgressable*>	m_listProgressable;
	list<cIControlable*>	m_listControlable;

	DWORD					m_CurrFrameTick;	// Milli Sec	
	DWORD					m_PrevFrameTick;	// Milli Sec	
	DWORD					m_AccumFrameTick;	// Milli Sec

	cWinInput*				m_pInput;	
	cD3DGraphics*			m_pGraphics;
	
	
	
public:	

	// 일반함수
	BOOL					ProcessWindowMessage();		//윈도우 메시지 루프
	BOOL					AddObject(IUnknownObject* pIUnknownObject);

	// Window관련
	void					SetTitleName( LPCTSTR lpszString );
	void					ScreenToClient(LPRECT lpRect) const;
	void					ClientToScreen(LPRECT lpRect) const;
	void					MoveWindow(int x,int y,int nWidth,int nHeight,BOOL bRepaint = TRUE );
	void					MoveWindow(LPCRECT lpRect,BOOL bRepaint = TRUE );
	
	DWORD					GetStyle() const;
	DWORD					GetExStyle() const;

	// Tick관련
	DWORD					GetCurrFrameTick() const { return m_CurrFrameTick; }
	DWORD					GetPrevFrameTick() const { return m_PrevFrameTick; }
	DWORD					GetAccumFrameTick() const { return m_AccumFrameTick; }
	
	// get/set
	cD3DGraphics*			GetRenderer() const { return m_pGraphics; }
	cWinInput*				GetInput() const { return m_pInput; }
	const RECT&					GetClient()  { return m_rtClient; }
	int						GetClientWidth() const { return m_rtClient.right-m_rtClient.left; }
	int						GetClientHeight() const { return m_rtClient.bottom-m_rtClient.top; }

	// 가상함수
	virtual BOOL 			OnWM_Keyboard(MSG& msg);	// 메시지를 처리했으면 TRUE를 리턴한다.
	virtual BOOL 			OnWM_Mouse(MSG& msg);		// 메시지를 처리했으면 TRUE를 리턴한다.
	virtual BOOL 			OnWM_General(MSG& msg);		// 메시지를 처리했으면 TRUE를 리턴한다.

	// 순수 가상 함수
	virtual void 			OnInitApplication()=0;	
	virtual	void 			OnCloseApplication()=0;

	//cFramework 구현
	virtual void 			InitApplication();
	virtual void 			Run();	
	virtual void 			CloseApplication();

	//cD3DFramework
	virtual void 			ProcessProgress();
	virtual void 			ProcessRender();

	virtual void			OnRenderStart();
	virtual void			OnRenderEnd();
};

extern cD3DFramework*	g_pD3DFramework;