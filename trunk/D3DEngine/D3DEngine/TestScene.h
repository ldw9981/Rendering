#pragma once
#include "./Scene/Scene.h"
#include "./Common/Interface.h"
#include "./Input/WinInput.h"
#include "./SceneGraph/CameraNode.h"

class cObjDragon;
class cGUIButton;
class cBackWall;
class cTestScene :
	public cScene
{
public:
	cTestScene(void);
	virtual ~cTestScene(void);
private:
	cCameraNode			m_Camera;
	cObjDragon*			m_pDragon;

public:
	// cScene
	virtual void Open(void* arg);
	virtual void Close();
	// IControlable
	virtual void Control();
	
	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();

	// INotifiable
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
