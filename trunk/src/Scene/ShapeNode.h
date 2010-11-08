#pragma once
#include "SceneNode.h"

class cShapeNode :
	public cSceneNode
{
public:
	cShapeNode(void);
	virtual ~cShapeNode(void);

protected:
	UINT	m_NUMLINE;
	
public:	
	virtual void	Update(DWORD elapseTime);
	virtual void	Render();

};
