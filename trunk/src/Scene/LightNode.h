#pragma once
#include "SceneGraphNode.h"

class cLightNode :
	public SceneGraphNode
{
public:
	cLightNode(void);
	virtual ~cLightNode(void);

public:
	// cObject
	virtual void	Init();
	virtual void	Uninit();

	virtual void	Update(DWORD elapseTime);	
};
