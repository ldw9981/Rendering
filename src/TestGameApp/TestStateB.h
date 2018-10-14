#pragma once
#include "Framework/View.h"
#include "Graphics/CameraNode.h"

class cObjDragon;
class cObjTank;
class TestStateB :
	public Sophia::cView
{
public:
	TestStateB(void);
	virtual ~TestStateB(void);
public:
	//cCameraNode			m_Camera;
	BOOL				m_bControlCamera;
	Sophia::Entity*			m_pDragon;
	Sophia::Entity*			m_pTank;
	Sophia::Entity*			m_pAirPlaneBake;
public:
	// cScene
	virtual void Enter();
	virtual void Leave();
	// cIControlable
	virtual void Control(DWORD elapseTime);

	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender(DWORD elapseTime);
};

