#pragma once
#include "SceneGraphNode.h"
#include "../Geometrics/Sphere.h"

class cHelperNode :
	public cSceneGraphNode
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
