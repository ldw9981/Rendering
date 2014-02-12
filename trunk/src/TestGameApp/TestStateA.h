#pragma once
#include "Framework/View.h"
#include "Scene/CameraNode.h"
#include "Scene/ZTerrain.h"

class cGUIButton;
class cObjTank;
class TestStateA :
	public cView	
{
public:
	TestStateA(void);
	virtual ~TestStateA(void);
private:
	//cCameraNode	m_Camera;
	Entity*		m_pTank;
	ZTerrain*	m_pZTerrain;
	BOOL		m_bControlCamera;
public:	
	virtual void Enter();
	virtual void Leave();

	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Control();

	// cINotifiable
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
