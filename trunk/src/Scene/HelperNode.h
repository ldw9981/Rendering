#pragma once
#include "SceneNode.h"
#include "Math/Sphere.h"

class cHelperNode :
	public cSceneNode
{
public:
	cHelperNode(void);
	virtual ~cHelperNode(void);
private:

public:
	virtual void	Render();
};
