#pragma once
#include "Scene/View.h"
#include "Scene/CameraNode.h"
#include "Scene/TerrainNode.h"
#include "Scene/ZTerrain.h"

class cGUIButton;
class cObjTank;
class cTerrainNode;
class TestStateA :
	public cView	
{
public:
	TestStateA(void);
	virtual ~TestStateA(void);
private:
	cCameraNode	m_Camera;
	cTerrainNode* m_pTerrain;

	cObjTank*	m_pTank;
	ZTerrain*	m_pZTerrain;
	BOOL		m_bControlCamera;
public:	
	virtual void Enter();
	virtual void Leave();

	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Control();

	// cINotifiable
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};

