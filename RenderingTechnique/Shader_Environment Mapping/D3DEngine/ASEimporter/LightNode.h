#pragma once
#include "SceneNode.h"

class cLightNode :
	public cSceneNode
{
public:
	cLightNode(void);
	virtual ~cLightNode(void);

public:
	virtual BOOL	LoadObjInfo(cASEParser* pParser);


	// cObject
	virtual void	Init();
	virtual void	Uninit();

	virtual void	Update();
	virtual void	Render();
};
