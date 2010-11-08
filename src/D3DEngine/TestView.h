#pragma once
#include "Scene/View.h"
#include "Foundation/Interface.h"
#include "WinInput/WinInput.h"
#include "Scene/CameraNode.h"

class cObjDragon;
class cGUIButton;
class cBackWall;
class cTestView :
	public cView
{
public:
	cTestView(void);
	virtual ~cTestView(void);
private:
	cCameraNode			m_Camera;
	cObjDragon*			m_pDragon;

public:
	// cScene
	virtual void Open(void* arg);
	virtual void Close();
	// cIControlable
	virtual void Control();
	
	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();

	// cINotifiable
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
