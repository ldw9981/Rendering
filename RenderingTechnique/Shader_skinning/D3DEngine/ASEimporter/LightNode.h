#pragma once
#include "SceneNode.h"

class cLightNode :
	public cSceneNode
{
public:
	cLightNode(void);
	virtual ~cLightNode(void);

public:


	// cObject
	virtual void	Init();
	virtual void	Uninit();

	virtual void	Update(DWORD elapseTime);
	virtual void	Render();
};
