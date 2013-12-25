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
	cRendererQueue			m_listRenderQueue[16];
	cRendererQueue			m_listRenderQueueSkinned[16];
	cRendererQueue			m_listRenderTerrain;
public:
	virtual void ProcessRender();
	virtual void Update(DWORD elapseTime);
	virtual void Render();
};