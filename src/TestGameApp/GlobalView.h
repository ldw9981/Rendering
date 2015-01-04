#pragma once
#include "Framework/View.h"
#include "Graphics/CameraNode.h"

namespace Sophia
{
	class cGUIFont;
	class cGUIButton;
}


class cGlobalView :
	public Sophia::cView
{
public:
	cGlobalView(void);
	virtual ~cGlobalView(void);
public:
	Sophia::cGUIButton* m_pBtNextScene;
	Sophia::cGUIButton* m_pBtExit;
	Sophia::cGUIFont*	m_pFont;

public:
	// cScene
	virtual void Enter();
	virtual void Leave();

	virtual void ProcessRender();

};
