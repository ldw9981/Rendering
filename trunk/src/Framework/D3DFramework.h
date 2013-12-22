#pragma once
#include "StaticHWND.h"
#include "D3D9Server/StaticD3DDevice9.h"
#include "./Framework/EnvironmentVariable.h"
#include "FpsMng.h"
#include "Scene/RendererQueue.h"


class cD3DFramework;

class		IUnknownObject;
class		IRenderable;
class		IUpdatable;
class		IControlable;

namespace D3D9
{
	class Server;
}	

class cResourceMng;
class cWinInput;
class cViewMng;
class cView;

class cD3DFramework :	
	private StaticD3DDEVICE9,
	private StaticHWND
{
public:
	cD3DFramework(const char* szTitleName,BOOL m_bFullScreen,int nWidth,int nHeight);
	virtual ~cD3DFramework(void);

public:

	HINSTANCE				m_hInstance;		 	 

protected:	
	
	WNDCLASS				m_wndclass;			// 메인 윈도우 클래스
	const char*				m_szClassName;		// 윈도우 클래스 등록 이름
	const char*				m_szTitleName;		// 윈도우 타이틀 이름
	RECT					m_RequestRect;
	RECT					m_AdjustRect;
	

	BOOL					m_bFullScreen;
	BOOL					m_bQuitLoop;

	// 이구조를 사용하면 추가될 객체의 Update/Render/Control호출 코드를 작성하지 않아도되며 추가순서에 따른 호출순서도 보장한다.
	std::list<IRenderable*>		m_listRenderable;
	std::list<IUpdatable*>		m_listProgressable;
	std::list<IControlable*>	m_listControlable;

	DWORD					m_CurrFrameTime;	// Milli Sec	
	DWORD					m_PrevFrameTime;	// Milli Sec	
	DWORD					m_AccumFrameTime;	// Milli Sec
	DWORD					m_DeltaFrameTime;

	// Server
	D3D9::Server*			m_pD3D9Server;
	cWinInput*				m_pInput;

	// Manager
	cResourceMng*			m_pResourceMng;
	CFpsMng					m_FpsMng;
	cView*					m_pView;
	
	
	static EnvironmentVariable m_sEnvironmentVariable;

public:	

	void					InitWindow();

	void					QuitLoop() { m_bQuitLoop = TRUE; }
	// 일반함수
	BOOL					ProcessWindowMessage();		//윈도우 메시지 루프
	
	void					AttachObject(IUnknownObject* pIUnknownObject);
	void					DettachObject(IUnknownObject* pIUnknownObject);


	// Window관련
	void					SetTitleName( const char* lpszString );
	void					ScreenToClient(LPRECT lpRect) const;
	void					ClientToScreen(LPRECT lpRect) const;
	void					MoveWindow(int x,int y,int nWidth,int nHeight,BOOL bRepaint = TRUE );
	void					MoveWindow(LPCRECT lpRect,BOOL bRepaint = TRUE );	
	DWORD					GetStyle() const;
	DWORD					GetExStyle() const;

	// Tick관련
	DWORD					GetCurrFrameTime() const { return m_CurrFrameTime; }
	DWORD					GetPrevFrameTime() const { return m_PrevFrameTime; }
	DWORD					GetAccumFrameTime() const { return m_AccumFrameTime; }
	
	// get/set
	cView*					GetView() const { return m_pView; }
	//cResourceMng*			GetResourceMng() const { return m_pResourceMng; }
	cWinInput*				GetInput() const { return m_pInput; }

	const RECT&				GetRequestRect()  { return m_RequestRect; }
	int						GetRequestRectWidth() const { return m_RequestRect.right-m_RequestRect.left; }
	int						GetRequestRectHeight() const { return m_RequestRect.bottom-m_RequestRect.top; }




	// 가상함수
	virtual BOOL 			OnWM_Keyboard(MSG& msg);	// 메시지를 처리했으면 TRUE를 리턴한다.
	virtual BOOL 			OnWM_Mouse(MSG& msg);		// 메시지를 처리했으면 TRUE를 리턴한다.
	virtual BOOL 			OnWM_General(MSG& msg);		// 메시지를 처리했으면 TRUE를 리턴한다.


	//cFramework 구현
	virtual bool 			Open();
	virtual void 			Run();	
	virtual void 			Close();

	//cD3DFramework
	virtual void			Control();
	virtual void 			Update(DWORD elapseTime);
	virtual void 			Render();


};

extern cD3DFramework*	g_pApp;