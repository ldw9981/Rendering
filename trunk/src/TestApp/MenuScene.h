#pragma once
#include "../Scene/Scene.h"
#include "../Scene/CameraNode.h"
#include "../Scene/TerrainNode.h"
#include "../Scene/ZTerrain.h"

// forward declare
class cGUIButton;
class cObjTank;
class ZTerrain;

class cMenuScene :
	public Scene	
{
public:
	cMenuScene(void);
	virtual ~cMenuScene(void);
private:
	CameraNode	m_Camera;
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

	// INotifiable
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
