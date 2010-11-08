#pragma once

#include "Scene/SceneNode.h"

#include "../Scene/RendererQueue.h"

class cTopRenderable:
	public cSceneNode,
	public IRenderable	
{
public:
	cTopRenderable();
	~cTopRenderable();

private:
	cRendererQueue	m_RendererQueue;

public:
	virtual void ProcessRender();
	virtual void Update(DWORD elapseTime);
	virtual void Render();
};