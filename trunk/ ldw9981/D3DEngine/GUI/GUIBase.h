#pragma once
#include "../Common/Interface.h"
#include "../Input/WinInput.h"
#include "../Framework/StaticD3DDevice9.h"

class cGUIBase:
	public IUnknownObject,
	public cIRenderable,
	public cIRenderer,
	public cIUpdatable,
	public cIControlable,
	public cStaticD3DDEVICE9,
	public cStaticWinInput
{
public:
	cGUIBase(void);
	virtual ~cGUIBase(void);
private:
	cINotifiable*		m_pMediator;
public:
	cINotifiable*		GetMediator() const { return m_pMediator; }
	void				SetMediator(cINotifiable* val) { m_pMediator = val; }
	virtual void 		ProcessRender();
 	virtual void 		Render()=0;
 	virtual void 		Update(DWORD elapsetime)=0;
 	virtual void 		Control()=0;
};
