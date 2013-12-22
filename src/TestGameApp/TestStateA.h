#pragma once

#include "Scene/View.h"
#include "Scene/CameraNode.h"

class cObjTank;
class TestStateA :
	public cView
{
public:
	TestStateA(void);
	virtual ~TestStateA(void);
public:
	cCameraNode			m_Camera;
	cObjTank*	m_pTank;
	// cScene
	virtual void Enter();
	virtual void Leave();
	// cIControlable
	virtual void Control();
	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
};

