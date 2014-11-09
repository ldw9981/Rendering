#pragma once
#include "Framework/View.h"
#include "Foundation/Interface.h"
#include "Input/Input.h"
#include "Scene/CameraNode.h"

class cObjDragon;
class cObjTank;
class cGUIButton;
class cBackWall;


class TestStateA;
class TestStateB;
class cGlobalView;


class cTestView :
	public Sophia::cView
{
public:
	cTestView(void);
	virtual ~cTestView(void);
private:
	TestStateA*	m_pTestStateA;
	TestStateB*	m_pTestStateB;
	//cCameraNode			m_Camera;
public:
	cGlobalView* m_pGlobalButtonScene;
	Sophia::Entity*			m_pP38;
public:
	// cScene
	virtual void Enter();
	virtual void Leave();
	// cIControlable
	virtual void Control();
	
	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();

	// cINotifiable
	virtual void Notify(Sophia::cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
