#pragma once

#include "../SceneGraph/SceneGraphNode.h"

#include "../Scene/RendererQueue.h"

class cTopRenderable:
	public cSceneGraphNode,
	public IRenderable	
{
public:
	cTopRenderable();
	~cTopRenderable();

private:
	cRendererQueue	m_RendererQueue;

public:

	virtual void Init();
	virtual void Uninit();
	virtual void ProcessRender();
	virtual void Update(DWORD elapseTime);
};