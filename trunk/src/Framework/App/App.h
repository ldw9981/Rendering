#pragma once
/*
#pragma comment(lib, "D3d9.lib")
#pragma comment(lib, "D3dx9.lib") 
*/
#include "SharedHWND.h"
#include "../../D3D9/SharedDevice.h"
#include "../../Framework/Interface.h"
#include "../../Foundation/Trace.h"

#include <list>
#include <d3d9.h>
#include <d3dx9math.h>


namespace Framework
{
	class		IUnknown;
	class		IRenderable;
	class		IUpdatable;
	class		IControlable;
}


using namespace std;


class ResourceMng;
class WinInput;
class SceneMng;
class cDebugInfoScene;

namespace Application
{

class App :
	public SharedLPDIRECT3DDEVICE9,
	public SharedHWND
{
public:
	App(LPCWSTR szTitleName,BOOL m_bFullScreen,int nWidth,int nHeight);
	virtual ~App(void);

	/// get instance pointer
	static App*				Instance();
public:

	HINSTANCE				m_hInstance;		 	 

protected:	
	static					App* Singleton;
	WNDCLASS				m_wndclass;			// ���� ������ Ŭ����
	LPCTSTR					m_szClassName;		// ������ Ŭ���� ��� �̸�
	LPCTSTR					m_szTitleName;		// ������ Ÿ��Ʋ �̸�
	RECT					m_RequestRect;
	RECT					m_AdjustRect;
	

	BOOL					m_bFullScreen;
	BOOL					m_bQuitLoop;

	list<Framework::IRenderable*>		m_listRenderable;
	list<Framework::IUpdatable*>		m_listProgressable;
	list<Framework::IControlable*>		m_listControlable;

	DWORD					m_CurrFrameTime;	// Milli Sec	
	DWORD					m_PrevFrameTime;	// Milli Sec	
	DWORD					m_AccumFrameTime;	// Milli Sec
	DWORD					m_DeltaFrameTime;

	WinInput*				m_pInput;	
	ResourceMng*			m_pResourceMng;
	SceneMng*				m_pSceneMng;
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
	
	void					AttachObject(Framework::IUnknown* pObject);
	void					DettachObject(Framework::IUnknown* pObject);
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
	SceneMng*				GetSceneMng() const { return m_pSceneMng; }
	ResourceMng*			GetResourceMng() const { return m_pResourceMng; }
	WinInput*				GetInput() const { return m_pInput; }

	const RECT&				GetRequestRect()  { return m_RequestRect; }
	int						GetRequestRectWidth() const { return m_RequestRect.right-m_RequestRect.left; }
	int						GetRequestRectHeight() const { return m_RequestRect.bottom-m_RequestRect.top; }

	cDebugInfoScene*		GetDebugInfoScene() const { return m_pDebugInfoScene; }


	// �����Լ�
	virtual BOOL 			OnWM_Keyboard(MSG& msg);	// �޽����� ó�������� TRUE�� �����Ѵ�.
	virtual BOOL 			OnWM_Mouse(MSG& msg);		// �޽����� ó�������� TRUE�� �����Ѵ�.
	virtual BOOL 			OnWM_General(MSG& msg);		// �޽����� ó�������� TRUE�� �����Ѵ�.
	
	//cFramework ����
	virtual BOOL 			Open();
	virtual void 			Run();	
	virtual void 			Close();

	//cD3DFramework
	virtual void			ProcessControlableList();
	virtual void 			ProcessProgressableList(DWORD elapseTime);
	virtual void 			ProcessRenderableList();


};
inline
App*
App::Instance()
{
	ASSERT(0 != App::Singleton);
	return App::Singleton;
}
}