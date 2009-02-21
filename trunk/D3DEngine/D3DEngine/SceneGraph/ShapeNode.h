#pragma once
#include "SceneGraphNode.h"

class cShapeNode :
	public cSceneGraphNode
{
public:
	cShapeNode(void);
	virtual ~cShapeNode(void);

protected:
	UINT	m_NUMLINE;
	
public:
	
	// cObject
	virtual void	Init();	
	virtual void	Uninit();

	virtual void	Update(DWORD elapseTime);
	virtual void	Render();

};
