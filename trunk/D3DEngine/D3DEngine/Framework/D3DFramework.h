#pragma once
#include "StaticHWND.h"
#include "StaticD3DDevice9.h"
#include "framework.h"


class cD3DFramework;

class		IUnknownObject;
class		IRenderable;
class		IUpdatable;
class		IControlable;


class cResourceMng;
class cWinInput;
class cSceneMng;
class cDebugInfoScene;
class cD3DFramework :
	public cFramework,
	private cStaticD3DDEVICE9,
	private cStaticHWND
{
public:
	cD3DFramework(LPCWSTR szTitleName,BOOL m_bFullScreen,int nWidth,int nHeight);
	virtual ~cD3DFramework(void);

public:

	HINSTANCE				m_hInstance;		 	 
private:	
	
	WNDCLASS				m_wndclass;			// ���� ������ Ŭ����
	LPCTSTR					m_szClassName;		// ������ Ŭ���� ��� �̸�
	LPCTSTR					m_szTitleName;		// ������ Ÿ��Ʋ �̸�
	RECT					m_RequestRect;
	RECT					m_AdjustRect;
	
	TCHAR					m_CurrDirectory[1024];

	BOOL					m_bFullScreen;
	BOOL					m_bQuitLoop;

	list<IRenderable*>		m_listRenderable;
	list<IUpdatable*>		m_listProgressable;
	list<IControlable*>	m_listControlable;

	DWORD					m_CurrFrameTime;	// Milli Sec	
	DWORD					m_PrevFrameTime;	// Milli Sec	
	DWORD					m_AccumFrameTime;	// Milli Sec
	DWORD					m_DeltaFrameTime;

	cWinInput*				m_pInput;	
	cResourceMng*			m_pResourceMng;
	cSceneMng*				m_pSceneMng;
	cDebugInfoScene*		m_pDebugInfoScene;

	// D3D �����߰�	
	D3DVIEWPORT9			m_ViewPortInfo;
	D3DPRESENT_PARAMETERS	m_D3DPP;         // Parameters for CreateDevice/Reset
	LPDIRECT3D9				m_pD3D9;
public:	
	// D3D �ʱ�ȭ�Լ� 
	void					InitD3D();
	
	void					QuitLoop() { m_bQuitLoop = TRUE; }
	// �Ϲ��Լ�
	BOOL					ProcessWindowMessage();		//������ �޽��� ����
	
	void					AttachObject(IUnknownObject* pIUnknownObject);
	void					DettachObject(IUnknownObject* pIUnknownObject);
	// D3D
	void					InitD3DWindowMode();

	// Window����
	void					SetTitleName( LPCTSTR lpszString );
	void					ScreenToClient(LPRECT lpRect) const;
	void					ClientToScreen(LPRECT lpRect) const;
	void					MoveWindow(int x,int y,int nWidth,int nHeight,BOOL bRepaint = TRUE );
	void					MoveWindow(LPCRECT lpRect,BOOL bRepaint = TRUE );	
	DWORD					GetStyle() const;
	DWORD					GetExStyle() const;

	// Tick����
	DWORD					GetCurrFrameTime() const { return m_CurrFrameTime; }
	DWORD					GetPrevFrameTime() const { return m_PrevFrameTime; }
	DWORD					GetAccumFrameTime() const { return m_AccumFrameTime; }
	
	// get/set
	cSceneMng*				GetSceneMng() const { return m_pSceneMng; }
	cResourceMng*			GetResourceMng() const { return m_pResourceMng; }
	cWinInput*				GetInput() const { return m_pInput; }

	const RECT&				GetRequestRect()  { return m_RequestRect; }
	int						GetRequestRectWidth() const { return m_RequestRect.right-m_RequestRect.left; }
	int						GetRequestRectHeight() const { return m_RequestRect.bottom-m_RequestRect.top; }

	cDebugInfoScene*		GetDebugInfoScene() const { return m_pDebugInfoScene; }


	// �����Լ�
	virtual BOOL 			OnWM_Keyboard(MSG& msg);	// �޽����� ó�������� TRUE�� �����Ѵ�.
	virtual BOOL 			OnWM_Mouse(MSG& msg);		// �޽����� ó�������� TRUE�� �����Ѵ�.
	virtual BOOL 			OnWM_General(MSG& msg);		// �޽����� ó�������� TRUE�� �����Ѵ�.

	// ���� ���� �Լ�
	virtual void 			OnInitApplication()=0;	
	virtual	void 			OnCloseApplication()=0;

	//cFramework ����
	virtual void 			InitApplication();
	virtual void 			Run();	
	virtual void 			CloseApplication();

	//cD3DFramework
	virtual void			ProcessControlableList();
	virtual void 			ProcessProgressableList(DWORD elapseTime);
	virtual void 			ProcessRenderableList();


};

extern cD3DFramework*	g_pD3DFramework;