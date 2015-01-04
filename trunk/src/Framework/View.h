#pragma once
#include "Foundation/Interface.h"
#include "Graphics/CameraNode.h"
#include "Framework/ViewMng.h"
#include "Graphics/Entity.h"
#include "Graphics/World.h"
namespace Sophia
{

class cCameraNode;
class cView:
	public IUnknownObject,
	public IUpdatable,
	public IRenderable,
	public IRenderer,
	public IControlable,
	public INotifiable,
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
//	D3DVIEWPORT9			m_ViewPortInfo;	
	std::string				m_strName;
	
public:
	Sophia::World					m_graphicWorld;
	cViewMng				m_ViewState;
public:
	cViewMng& GetState() { return m_ViewState; }
	cView* GetParentView() const { return m_pParentView; }
	void SetParentView(cView* val) { m_pParentView = val; }


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
}