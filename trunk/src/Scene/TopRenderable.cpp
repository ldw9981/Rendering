#include "stdafx.h"
#include "TopRenderable.h"
#include "Scene/CameraNode.h"

cTopRenderable::cTopRenderable()
{

}

cTopRenderable::~cTopRenderable()
{

}

void cTopRenderable::ProcessRender()
{
	cCameraNode* pActiveCamera = cCameraNode::GetActiveCamera();
	CullRendererTraversal(&m_RendererQueue,pActiveCamera);
	m_RendererQueue.Render();
}


void cTopRenderable::Update( DWORD elapseTime )
{	
	UpdateChildren(elapseTime);
}

void cTopRenderable::Render()
{

}
