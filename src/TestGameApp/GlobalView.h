#pragma once
#include "Framework/View.h"
#include "Scene/CameraNode.h"

class cGUIFont;
class cGUIButton;

class cGlobalView :
	public cView
{
public:
	cGlobalView(void);
	virtual ~cGlobalView(void);
public:
	cGUIButton* m_pBtNextScene;
	cGUIButton* m_pBtExit;
	cGUIFont*	m_pFont;

public:
	// cScene
	virtual void Enter();
	virtual void Leave();

	virtual void ProcessRender();

};
