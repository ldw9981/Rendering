#pragma once
#include "Framework/View.h"
#include "Scene/CameraNode.h"

class cObjDragon;
class cObjTank;
class TestStateB :
	public cView
{
public:
	TestStateB(void);
	virtual ~TestStateB(void);
public:
	//cCameraNode			m_Camera;
	BOOL				m_bControlCamera;
	Entity*			m_pDragon;
	Entity*			m_pTank;
	Entity*			m_pAirPlaneBake;
public:
	// cScene
	virtual void Enter();
	virtual void Leave();
	// cIControlable
	virtual void Control();

	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
};

