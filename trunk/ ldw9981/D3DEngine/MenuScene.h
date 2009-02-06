#pragma once
#include "./Scene/Scene.h"
#include "./SceneGraph/CameraNode.h"
#include "./OutDoor/TerrainNode.h"
#include "./OutDoor/ZTerrain.h"

class cGUIButton;
class cObjTank;
class cTerrainNode;
class cMenuScene :
	public cScene	
{
public:
	cMenuScene(void);
	virtual ~cMenuScene(void);
private:
	cCameraNode	m_Camera;
	cTerrainNode* m_pTerrain;
	
	cObjTank*	m_pTank;
	ZTerrain*	m_pZTerrain;
	BOOL		m_bControlCamera;
public:	
	virtual void Open(void* arg);
	virtual void Close();

	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Control();

	// cINotifiable
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
