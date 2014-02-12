#pragma once
#include "Foundation/Interface.h"
#include "Input/Input.h"

class World;
class cGUIBase:
	public IUnknownObject,
	public IRenderable,
	public IRenderer,
	public IUpdatable,
	public IControlable
{
public:
	cGUIBase(void);
	virtual ~cGUIBase(void);
protected:
	INotifiable*		m_pMediator;
	World*				m_pWorld;
	
public:
	void				SetWorld(World* val) { m_pWorld = val; }
	INotifiable*		GetMediator() const { return m_pMediator; }
	void				SetMediator(INotifiable* val) { m_pMediator = val; }
	virtual void 		ProcessRender();
 	virtual void 		Render()=0;
 	virtual void 		Update(DWORD elapsetime)=0;
 	virtual void 		Control()=0;
};
