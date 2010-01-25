#pragma once
#include "../Framework/App/App.h"
#include "../Framework/Input/WinInput.h"
#include "../D3D9/SharedDevice.h"
#include "../Framework/Interface.h"

namespace Framework
{
class INotifiable;
}



class cGUIBase:
	public Framework::IUnknown,
	public Framework::IRenderable,
	public Framework::IRenderer,
	public Framework::IUpdatable,
	public Framework::IControlable,
	public SharedLPDIRECT3DDEVICE9,
	public SharedWinInput
{
public:
	cGUIBase(void);
	virtual ~cGUIBase(void);
private:
	Framework::INotifiable*		m_pMediator;
public:
	Framework::INotifiable*		GetMediator() const { return m_pMediator; }
	void				SetMediator(Framework::INotifiable* val) { m_pMediator = val; }
	virtual void 		ProcessRender();
 	virtual void 		Render()=0;
 	virtual void 		Update(DWORD elapsetime)=0;
 	virtual void 		Control()=0;
};
