#pragma once
#include "SceneGraphNode.h"
#include "../Math/Sphere.h"

class cHelperNode :
	public SceneGraphNode
{
public:
	cHelperNode(void);
	virtual ~cHelperNode(void);
private:

public:
	// cObject
	virtual void	Init();
	virtual void	Uninit();
	
	virtual void	Update(DWORD elapseTime);	
};
