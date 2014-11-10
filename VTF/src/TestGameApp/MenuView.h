#pragma once
#include "Framework/View.h"
#include "Scene/CameraNode.h"
#include "Scene/ZTerrain.h"

#define STRESS 256


class cGUIButton;
class cObjTank;
class cObjDragon;
class Entity;
class cMenuView :
	public Sophia::cView	
{
public:
	cMenuView(void);
	virtual ~cMenuView(void);
private:
	//cCameraNode	m_Camera;
	Sophia::Entity*	m_pTank;
	Sophia::ZTerrain*	m_pZTerrain;
	BOOL		m_bControlCamera;
	Sophia::Entity*			m_pDragon;
	Sophia::Entity*			m_pAirPlaneBake;
	Sophia::Entity*			m_pHouse[STRESS];
	Sophia::Entity*			m_pSkinned[STRESS];
	bool		m_instancing;
public:	
	virtual void Enter();
	virtual void Leave();

	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Control();

	// cINotifiable
	virtual void Notify(Sophia::cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
