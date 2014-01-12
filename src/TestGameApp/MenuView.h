#pragma once
#include "Scene/View.h"
#include "Scene/CameraNode.h"
#include "Scene/ZTerrain.h"

class cGUIButton;
class cObjTank;
class cObjDragon;
class cMenuView :
	public cView	
{
public:
	cMenuView(void);
	virtual ~cMenuView(void);
private:
	cCameraNode	m_Camera;
	cObjTank*	m_pTank;
	ZTerrain*	m_pZTerrain;
	BOOL		m_bControlCamera;
	cObjDragon*			m_pDragon;
	cObjDragon*			m_pAirPlaneBake;
	cObjDragon*			m_pHouse;
public:	
	virtual void Enter();
	virtual void Leave();

	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Control();

	// cINotifiable
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
