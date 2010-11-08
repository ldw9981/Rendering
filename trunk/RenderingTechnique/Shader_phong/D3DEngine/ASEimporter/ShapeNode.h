#pragma once
#include "SceneNode.h"

class cShapeNode :
	public cSceneNode
{
public:
	cShapeNode(void);
	virtual ~cShapeNode(void);

protected:
	int	m_NUMLINE;
	
public:
	
	// cObject
	virtual void Init();	
	virtual void	Uninit();

	virtual void	Update();
	virtual void	Render();

	// cSceneNode	
	virtual BOOL	LoadObjInfo(cASEParser* pParser);
};
