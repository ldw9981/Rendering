#pragma once
#include "../Common/Interface.h"
#include "../SceneGraph/CameraNode.h"
#include "../Framework/StaticD3DDevice9.h"
#include "../Input/WinInput.h"
#include "TopRenderable.h"

class cCameraNode;
class cScene:
	public IUnknownObject,
	public IUpdatable,
	public IRenderable,
	public IControlable,
	public INotifiable,
	public cStaticD3DDEVICE9,
	public cStaticWinInput
{
public:
	cScene(void);
	cScene(cScene* pParent);
	virtual ~cScene(void);
private:
	list<IRenderable*>		m_RenderableList;
	list<IUpdatable*>		m_ProgressableList;
	list<IControlable*>		m_ControlableList;

	cScene*					m_pParentScene;				// 부모 씬
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

	// IRenderable
	virtual void			ProcessRender();
	// IUpdatable
	virtual void			Update(DWORD elapseTime);

	virtual void			Control();

};
