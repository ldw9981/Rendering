#pragma once
#include "Foundation/EnvironmentVariable.h"
#include "FpsMng.h"
#include "Graphics/RendererQueue.h"


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
	HINSTANCE				m_hInstance;	
protected:	

	RECT					m_RequestRect;

	BOOL					m_bFullScreen;
	BOOL					m_bQuitLoop;

	// �̱����� ����ϸ� �߰��� ��ü�� Update/Render/Controlȣ�� �ڵ带 �ۼ����� �ʾƵ��Ǹ� �߰������� ���� ȣ������� �����Ѵ�.
	std::list<IRenderable*>		m_listRenderable;
	std::list<IUpdatable*>		m_listProgressable;
	std::list<IControlable*>	m_listControlable;

	DWORD					m_CurrFrameTime;	// Milli Sec	
	DWORD					m_PrevFrameTime;	// Milli Sec	
	DWORD					m_AccumFrameTime;	// Milli Sec
	DWORD					m_DeltaFrameTime;

	// Server
	Graphics*				m_pGraphics;
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
	// �Ϲ��Լ�
	
	
	void					AttachObject(IUnknownObject* pIUnknownObject);
	void					DettachObject(IUnknownObject* pIUnknownObject);



	const RECT&				GetRequestRect()  { return m_RequestRect; }
	int						GetRequestRectWidth() const { return m_RequestRect.right-m_RequestRect.left; }
	int						GetRequestRectHeight() const { return m_RequestRect.bottom-m_RequestRect.top; }

	// Tick����
	DWORD					GetCurrFrameTime() const { return m_CurrFrameTime; }
	DWORD					GetPrevFrameTime() const { return m_PrevFrameTime; }
	DWORD					GetAccumFrameTime() const { return m_AccumFrameTime; }
	
	// get/set
	cView*					GetView() const { return m_pView; }
	//cResourceMng*			GetResourceMng() const { return m_pResourceMng; }
	Input*					GetInput() const { return m_pInput; }
	Window*					GetWindow() const { return m_pWindow; }





	// �����Լ�
	virtual BOOL 			OnWM_Keyboard(MSG& msg);	// �޽����� ó�������� TRUE�� �����Ѵ�.
	virtual BOOL 			OnWM_Mouse(MSG& msg);		// �޽����� ó�������� TRUE�� �����Ѵ�.
	virtual BOOL 			OnWM_General(MSG& msg);		// �޽����� ó�������� TRUE�� �����Ѵ�.


	//cFramework ����
	virtual bool 			Initialize();
	virtual void 			Run();	
	virtual void 			Finalize();

	//cD3DFramework
	virtual void			Control();
	virtual void 			Update(DWORD elapseTime);
	virtual void 			Render();
};

extern cD3DFramework*	g_pApp;