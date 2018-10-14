#pragma once
#include "Framework/View.h"
#include "Graphics/CameraNode.h"
#include "Graphics/ZTerrain.h"

#define STRESS_NORMAL 124
#define STRESS_SKINNED 1024

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
	Sophia::Entity*	m_pGirl;
	Sophia::ZTerrain*	m_pZTerrain;
	BOOL		m_bControlCamera;

	Sophia::Entity*			m_pLightMap;
	Sophia::Entity*			m_pDragon;
	Sophia::Entity*			m_pAirPlaneBake;
	Sophia::Entity*			m_pHouse[STRESS_NORMAL];
	Sophia::Entity*			m_pSkinned[STRESS_SKINNED];
	bool		m_instancingNormal;
	bool		m_instancingSkinned;
	bool		m_showNormal;
	bool		m_showSkinned;

	Sophia::cSceneNode* m_pInputTarget;
public:	
	virtual void Enter();
	virtual void Leave();

	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender(DWORD elapseTime);
	virtual void Control(DWORD elapseTime);

	// cINotifiable
	virtual void Notify(Sophia::cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);

	void ControlInputTarget(DWORD elapseTime);
};
