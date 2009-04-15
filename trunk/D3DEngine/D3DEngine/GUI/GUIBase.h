#pragma once
#include "../Common/Interface.h"
#include "../Input/WinInput.h"
#include "../Framework/StaticD3DDevice9.h"

class cScene;
class cGUIBase:
	public IUnknownObject,
	public IRenderable,
	public IRenderer,
	public IUpdatable,
	public IControlable,
	public cStaticD3DDEVICE9,
	public cStaticWinInput
{
public:
	cGUIBase(void);
	virtual ~cGUIBase(void);
private:
	cScene*				m_pMediator;
public:
	cScene*				GetMediator() const { return m_pMediator; }
	void				SetMediator(cScene* val) { m_pMediator = val; }
	virtual void 		ProcessRender();
 	virtual void 		Render()=0;
 	virtual void 		Update(DWORD elapsetime)=0;
 	virtual void 		Control()=0;
};
