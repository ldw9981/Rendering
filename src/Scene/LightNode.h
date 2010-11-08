#pragma once
#include "SceneNode.h"

class cLightNode :
	public cSceneNode
{
public:
	cLightNode(void);
	virtual ~cLightNode(void);

public:
	virtual void	Update(DWORD elapseTime);	
	virtual void	Render();
};
