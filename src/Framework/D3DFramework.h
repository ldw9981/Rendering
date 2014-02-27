#pragma once
#include "Foundation/EnvironmentVariable.h"
#include "FpsMng.h"
#include "Graphics/RendererQueue.h"

namespace Sophia
{

class cD3DFramework;

class		IUnknownObject;
class		IRenderable;
class		IUpdatable;
class		IControlable;
class Graphics;
class cResourceMng;
class Input;
class cViewMng;
class cView;
class Window;
class CursorManager;

class cD3DFramework 
{
public:
	cD3DFramework(const char* szTitleName,BOOL m_bFullScreen,int nWidth,int nHeight);
	virtual ~cD3DFramework(void);

public:
	static cD3DFramework*   m_pInstance;
	HINSTANCE				m_hInstance;	
	HWND					m_hWndMain;
	HWND					m_hWndPresent;
protected:	
	RECT					m_RequestRect;

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
	Sophia::Graphics*				m_pGraphics;
	Input*					m_pInput;

	// Manager
	Window*					m_pWindow;
	cResourceMng*			m_pResourceMng;
	
	CFpsMng					m_FpsMng;
	CursorManager*			m_pCursorManager;
	cView*					m_pView;
	
	
	static EnvironmentVariable m_sEnvironmentVariable;

public:	

	

	void					QuitLoop() { m_bQuitLoop = TRUE; }
	// 일반함수
	
	
	void					AttachObject(IUnknownObject* pIUnknownObject);
	void					DettachObject(IUnknownObject* pIUnknownObject);



	const RECT&				GetRequestRect()  { return m_RequestRect; }
	int						GetRequestRectWidth() const { return m_RequestRect.right-m_RequestRect.left; }
	int						GetRequestRectHeight() const { return m_RequestRect.bottom-m_RequestRect.top; }

	// Tick관련
	DWORD					GetCurrFrameTime() const { return m_CurrFrameTime; }
	DWORD					GetPrevFrameTime() const { return m_PrevFrameTime; }
	DWORD					GetAccumFrameTime() const { return m_AccumFrameTime; }
	
	// get/set
	cView*					GetView() const { return m_pView; }
	//cResourceMng*			GetResourceMng() const { return m_pResourceMng; }
	Input*					GetInput() const { return m_pInput; }
	Window*					GetWindow() const { return m_pWindow; }





	// 가상함수
	virtual BOOL 			OnWM_Keyboard(MSG& msg);	// 메시지를 처리했으면 TRUE를 리턴한다.
	virtual BOOL 			OnWM_Mouse(MSG& msg);		// 메시지를 처리했으면 TRUE를 리턴한다.
	virtual BOOL 			OnWM_General(MSG& msg);		// 메시지를 처리했으면 TRUE를 리턴한다.


	//cFramework 구현
	virtual bool 			Initialize();
	virtual void 			Run();	
	virtual void 			Finalize();

	//cD3DFramework
	virtual void			Control();
	virtual void 			Update(DWORD elapseTime);
	virtual void 			Render();

	virtual void 			InitWindow();
};

extern cD3DFramework*	g_pApp;
}