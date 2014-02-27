#pragma once
#include "Framework/View.h"
class Sophia::Entity;
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

	Sophia::Entity*			m_pModel;
public:	
	virtual void Enter();
	virtual void Leave();

	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Control();

	void OpenASE(const char* path);
	void OpenAsset(const char* path);
	void SaveAsset();


	Sophia::Entity* GetEntity();
};

