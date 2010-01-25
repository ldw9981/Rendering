#pragma once
#include "../Framework/Interface.h"
#include "../Scene/CameraNode.h"
#include "../D3D9/SharedDevice.h"
#include "../Framework/Input/WinInput.h"
#include "TopRenderable.h"

class CameraNode;
class Scene:
	public Framework::IUnknown,
	public Framework::IUpdatable,
	public Framework::IRenderable,
	public Framework::IControlable,
	public Framework::INotifiable,
	public SharedLPDIRECT3DDEVICE9,
	public SharedWinInput
{
public:
	Scene(void);
	Scene(Scene* pParent);
	virtual ~Scene(void);
private:
	list<Framework::IRenderable*>		m_RenderableList;
	list<Framework::IUpdatable*>		m_ProgressableList;
	list<Framework::IControlable*>	m_ControlableList;

	Scene*					m_pParentScene;				// 부모 씬
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
	void					AttachObject(Framework::IUnknown* pIUnknownObject);
	void					DettachObject(Framework::IUnknown* pIUnknownObject);

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
