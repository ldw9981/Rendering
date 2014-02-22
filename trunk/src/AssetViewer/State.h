#pragma once
#include "Framework/View.h"

class State : public Sophia::cView
{
public:
	State(void);
	virtual ~State(void);
private:
	//cCameraNode	m_Camera;
	Sophia::Entity*	m_pTank;
	Sophia::ZTerrain*	m_pZTerrain;
	BOOL		m_bControlCamera;
	Sophia::Entity*			m_pDragon;
	Sophia::Entity*			m_pAirPlaneBake;
	Sophia::Entity*			m_pHouse[10];
public:	
	virtual void Enter();
	virtual void Leave();

	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Control();
};

