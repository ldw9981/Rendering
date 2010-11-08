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
	RECT					m_rtClient;
	

	BOOL					m_bFullScreen;

	BOOL					m_bQuitLoop;
	list<cIRenderable*>		m_listRenderable;
	list<cIProgressable*>	m_listProgressable;
	list<cIControlable*>	m_listControlable;

	DWORD					m_CurrFrameTime;	// Milli Sec	
	DWORD					m_PrevFrameTime;	// Milli Sec	
	DWORD					m_AccumFrameTime;	// Milli Sec
	DWORD					m_DeltaFrameTime;

	cWinInput*				m_pInput;	
	cD3DGraphics*			m_pGraphics;
	
	
	
public:	

	// �Ϲ��Լ�
	BOOL					ProcessWindowMessage();		//������ �޽��� ����
	BOOL					AddObject(IUnknownObject* pIUnknownObject);

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
	cD3DGraphics*			GetRenderer() const { return m_pGraphics; }
	cWinInput*				GetInput() const { return m_pInput; }
	const RECT&				GetClient()  { return m_rtClient; }
	int						GetClientWidth() const { return m_rtClient.right-m_rtClient.left; }
	int						GetClientHeight() const { return m_rtClient.bottom-m_rtClient.top; }

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
	virtual void 			ProcessProgress();
	virtual void 			ProcessRender();

	virtual void			OnRenderStart();
	virtual void			OnRenderEnd();
};

extern cD3DFramework*	g_pD3DFramework;