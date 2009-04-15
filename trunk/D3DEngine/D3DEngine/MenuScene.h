#pragma once
#include "./Scene/Scene.h"
#include "./SceneGraph/CameraNode.h"
#include "./OutDoor/TerrainNode.h"



class cGUIButton;
class cObjTank;
class cTerrainNodeNode;
class cMenuScene :
	public cScene	
{
public:
	cMenuScene(void);
	virtual ~cMenuScene(void);
private:
	cCameraNode	m_Camera;
	cTerrainNodeNode* m_pTerrain;
	
	cObjTank*	m_pTank;
	cTerrainNode*	m_pcTerrainNode;
	BOOL		m_bControlCamera;
public:	
	virtual void Open(void* arg);
	virtual void Close();

	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Control();

	// INotifiable
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
