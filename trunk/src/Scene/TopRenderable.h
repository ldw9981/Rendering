#pragma once

#include "../Scene/SceneGraphNode.h"

#include "../Scene/RendererQueue.h"

class cTopRenderable:
	public SceneGraphNode,
	public Framework::IRenderable	
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