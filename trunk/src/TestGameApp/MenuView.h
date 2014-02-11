#pragma once
#include "Framework/View.h"
#include "Scene/CameraNode.h"
#include "Scene/ZTerrain.h"

class cGUIButton;
class cObjTank;
class cObjDragon;
class Entity;
class cMenuView :
	public cView	
{
public:
	cMenuView(void);
	virtual ~cMenuView(void);
private:
	//cCameraNode	m_Camera;
	Entity*	m_pTank;
	ZTerrain*	m_pZTerrain;
	BOOL		m_bControlCamera;
	Entity*			m_pDragon;
	Entity*			m_pAirPlaneBake;
	Entity*			m_pHouse[10];
public:	
	virtual void Enter();
	virtual void Leave();

	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Control();

	// cINotifiable
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
