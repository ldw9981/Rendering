#pragma once
#include "Framework/View.h"
#include "Graphics/CameraNode.h"
#include "Graphics/ZTerrain.h"

class cGUIButton;
class cObjTank;
class TestStateA :
	public Sophia::cView	
{
public:
	TestStateA(void);
	virtual ~TestStateA(void);
private:
	//cCameraNode	m_Camera;
	Sophia::Entity*		m_pTank;
	Sophia::ZTerrain*	m_pZTerrain;
	BOOL		m_bControlCamera;
public:	
	virtual void Enter();
	virtual void Leave();

	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Control();

	// cINotifiable
	virtual void Notify(Sophia::cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};

