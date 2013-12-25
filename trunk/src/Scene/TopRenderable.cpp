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
	if (pActiveCamera)
	{
		//CullRendererIntoRendererQueue(pActiveCamera);
	}	
}


void cTopRenderable::Update( DWORD elapseTime )
{	
	UpdateChildren(elapseTime);
}

void cTopRenderable::Render()
{

}
