#pragma once
#include "Foundation/Interface.h"
#include "Scene/CameraNode.h"
#include "D3D9Server/StaticD3DDevice9.h"
#include "WinInput/WinInput.h"
#include "TopRenderable.h"
#include "Scene/ViewMng.h"

class cCameraNode;
class cView:
	public IUnknownObject,
	public IUpdatable,
	public IRenderable,
	public IRenderer,
	public IControlable,
	public INotifiable,
	private StaticD3DDEVICE9,
	public cStaticWinInput,
	public FiniteState
{
public:
	cView(void);
	virtual ~cView(void);
	friend class cViewMng;
protected:
	std::list<IRenderable*>		m_RenderableList;
	std::list<IUpdatable*>		m_ProgressableList;
	std::list<IControlable*>	m_ControlableList;

	cView*					m_pParentView;				// 부모 씬
	

	BOOL					m_bHide;					//입력을 받지않고 그리지않는다.	갱신은 한다.	
	D3DVIEWPORT9			m_ViewPortInfo;	
	std::string				m_strName;
public:
	cRendererQueue			m_listRenderQueue[16];
	cRendererQueue			m_listRenderQueueSkinned[16];
	cRendererQueue			m_listRenderTerrain;
	cRendererQueue			m_listRenderGUI;

	cRendererQueue			m_listShadowNormal;
	cRendererQueue			m_listShadowBlend;
	cViewMng				m_ViewState;
	cSceneNode				m_listScene;				// Transform 있는것들
public:
	cViewMng& GetState() { return m_ViewState; }
	cView* GetParentView() const { return m_pParentView; }
	void SetParentView(cView* val) { m_pParentView = val; }
	D3DVIEWPORT9&			GetViewPortInfo()  { return m_ViewPortInfo; }
	void					SetViewPortInfo(UINT x,UINT y,UINT width,UINT height);
	void					SetViewPortInfo(D3DVIEWPORT9& val) { m_ViewPortInfo = val; }
	void					SetViewPort();

	BOOL					GetHide() const { return m_bHide; }
	void					SetHide(BOOL val) { m_bHide = val; }
	void					AttachObject(IUnknownObject* pIUnknownObject);
	void					DettachObject(IUnknownObject* pIUnknownObject);
	virtual void			Enter();
	virtual void			Leave();

	// cIRenderable
	virtual void			ProcessRender();
	// cIUpdatable
	virtual void			Update(DWORD elapseTime);

	virtual void			Control();

	virtual void			Render();

	virtual void			Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
