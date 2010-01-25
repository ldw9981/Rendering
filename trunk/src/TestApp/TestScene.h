#pragma once
#include "../Scene/Scene.h"
#include "../../Framework/Interface.h"
#include "../../Framework/Input/WinInput.h"
#include "../Scene/CameraNode.h"

class cObjDragon;
class cGUIButton;
class cBackWall;
class TestScene 
	:public Scene
{
public:
	TestScene(void);
	virtual ~TestScene(void);
private:
	CameraNode			m_Camera;
	cObjDragon*			m_pDragon;

public:
	// Scene
	virtual void Open(void* arg);
	virtual void Close();
	// IControlable
	virtual void Control();
	
	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();

	// INotifiable
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
