#pragma once
#include "Foundation/Interface.h"
#include "Scene/CameraNode.h"
#include "D3D9Server/StaticD3DDevice9.h"
#include "WinInput/WinInput.h"
#include "TopRenderable.h"

class cCameraNode;
class cView:
	public IUnknownObject,
	public IUpdatable,
	public IRenderable,
	public IControlable,
	public INotifiable,
	private StaticD3DDEVICE9,
	public cStaticWinInput
{
public:
	cView(void);
	cView(cView* pParent);
	virtual ~cView(void);
private:
	std::list<IRenderable*>		m_RenderableList;
	std::list<IUpdatable*>		m_ProgressableList;
	std::list<IControlable*>	m_ControlableList;

	cView*					m_pParentScene;				// 부모 씬
	cTopRenderable			m_TopRenderable;

	BOOL					m_bHide;					//입력을 받지않고 그리지않는다.	갱신은 한다.
	
	D3DVIEWPORT9			m_ViewPortInfo;
	
public:
	
public:
	D3DVIEWPORT9&			GetViewPortInfo()  { return m_ViewPortInfo; }
	void					SetViewPortInfo(UINT x,UINT y,UINT width,UINT height);
	void					SetViewPortInfo(D3DVIEWPORT9& val) { m_ViewPortInfo = val; }
	void					SetViewPort();

	BOOL					GetHide() const { return m_bHide; }
	void					SetHide(BOOL val) { m_bHide = val; }
	void					AttachObject(IUnknownObject* pIUnknownObject);
	void					DettachObject(IUnknownObject* pIUnknownObject);

	void					ProcessControlableList();
	void					ProcessRenderableList();
	void					ProcessProgressableList(DWORD elapse);	

	virtual void			Open(void* arg)=0;
	virtual void			Close()=0;

	// cIRenderable
	virtual void			ProcessRender();
	// cIUpdatable
	virtual void			Update(DWORD elapseTime);

	virtual void			Control();

};
