#include "StdAfx.h"
#include "View.h"
#include "TopRenderable.h"
#include "D3D9Server/StaticD3DDevice9.h"
#include "Scene/CameraNode.h"
#include "D3D9Server/Server.h"

cView::cView(void)
:m_pParentView(NULL)
{
	m_bHide=FALSE;

	m_ViewPortInfo.MinZ = 0.0f;
	m_ViewPortInfo.MaxZ = 1.0f;
}

cView::~cView(void)
{
	
}

void cView::AttachObject( IUnknownObject* pIUnknownObject )
{
	// Control은 Scene에서 호출
	IControlable* pIC = dynamic_cast< IControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_ControlableList.push_back(pIC);
		pIC->m_ItControlable = --m_ControlableList.end();	
	}

	cSceneNode* pSceneGraphNode = dynamic_cast<cSceneNode*>(pIUnknownObject);
	if (pSceneGraphNode !=NULL)
	{
		m_listScene.AttachChildNode(pSceneGraphNode);
		return;
	}


	IRenderable* pIR = dynamic_cast<IRenderable*>(pIUnknownObject);
	if(pIR != NULL)
	{
		m_RenderableList.push_back(pIR);
		pIR->m_ItRenderable = --m_RenderableList.end();	
	}

	IUpdatable* pIP = dynamic_cast< IUpdatable *>(pIUnknownObject);
	if(pIP != NULL)
	{
		m_ProgressableList.push_back(pIP);
		pIP->m_ItProgressable = --m_ProgressableList.end();	
	}
}

void cView::DettachObject( IUnknownObject* pIUnknownObject )
{
	// Control은 Scene에서 호출
	IControlable* pIC = dynamic_cast< IControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_ControlableList.erase(pIC->m_ItControlable);
	}	

	cSceneNode* pSceneGraphNode = dynamic_cast<cSceneNode*>(pIUnknownObject);
	if (pSceneGraphNode !=NULL)
	{
		m_listScene.DettachChildNode(pSceneGraphNode);
		return;
	}

	IRenderable* pIR = dynamic_cast<IRenderable*>(pIUnknownObject);
	if(pIR != NULL)
	{
		m_RenderableList.erase(pIR->m_ItRenderable);
	}

	IUpdatable* pIP = dynamic_cast< IUpdatable *>(pIUnknownObject);
	if(pIP != NULL)
	{
		m_ProgressableList.erase(pIP->m_ItProgressable);
	}
}

void cView::Update( DWORD elapseTime )
{	
	m_listScene.Update(elapseTime);		// SceneGraph는 계층트리구조 부모우선 순회가 이루어져야한다.

	std::list<IUpdatable*>::iterator it=m_ProgressableList.begin();
	for ( ;it!=m_ProgressableList.end() ; ++it )
	{
		(*it)->Update(elapseTime);
	}		
	m_ViewState.Update(elapseTime);
}

void cView::Control()
{
	if (m_bHide)
		return;

	std::list<IControlable*>::iterator it_control=m_ControlableList.begin();
	for ( ;it_control!=m_ControlableList.end() ; ++it_control )
	{
		(*it_control)->Control();
	}	
	m_ViewState.Control();
}

void cView::ProcessRender()
{
	if (m_bHide)
		return;
	
	// scene 
	cCameraNode* pActiveCamera = cCameraNode::GetActiveCamera();
	if (pActiveCamera)
	{
		m_listScene.CullRendererIntoRendererQueue(this,pActiveCamera);
	}		
	D3D9::Server::g_pServer->Render(this);

	// gui 
	std::list<IRenderable*>::iterator it=m_RenderableList.begin();
	for ( ;it!=m_RenderableList.end() ; ++it )
	{
		(*it)->ProcessRender();	
	}
	
	// state 
	m_ViewState.ProcessRender();
}

void cView::SetViewPortInfo( UINT x,UINT y,UINT width,UINT height )
{
	if (m_pParentView)
	{ 
		m_ViewPortInfo.X= m_pParentView->m_ViewPortInfo.X + x;
		m_ViewPortInfo.Y= m_pParentView->m_ViewPortInfo.Y + y;
	}
	else
	{
		m_ViewPortInfo.X = x;
		m_ViewPortInfo.Y = y;
	}

	m_ViewPortInfo.Width=width;
	m_ViewPortInfo.Height=height;	
}

void cView::SetViewPort()
{
	m_pD3DDevice->SetViewport(&m_ViewPortInfo);				
}

void cView::Render()
{

}

void cView::Enter()
{

}

void cView::Leave()
{

}

void cView::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{

}
