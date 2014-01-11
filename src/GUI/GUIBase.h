#pragma once
#include "Foundation/Interface.h"
#include "WinInput/WinInput.h"
#include "D3D9Server/StaticD3DDevice9.h"

class cGUIBase:
	public IUnknownObject,
	public IRenderable,
	public IRenderer,
	public IUpdatable,
	public IControlable,
	public StaticD3DDEVICE9
{
public:
	cGUIBase(void);
	virtual ~cGUIBase(void);
private:
	INotifiable*		m_pMediator;
public:
	INotifiable*		GetMediator() const { return m_pMediator; }
	void				SetMediator(INotifiable* val) { m_pMediator = val; }
	virtual void 		ProcessRender();
 	virtual void 		Render()=0;
 	virtual void 		Update(DWORD elapsetime)=0;
 	virtual void 		Control()=0;
};
