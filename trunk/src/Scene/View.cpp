#include "StdAfx.h"
#include "View.h"
#include "TopRenderable.h"
#include "D3D9Server/StaticD3DDevice9.h"
#include "Scene/CameraNode.h"

cView::cView(void)
:m_pParentScene(NULL)
{
	m_bHide=FALSE;
}

cView::cView( cView* pParent )
:m_pParentScene(pParent)
{
	m_bHide=FALSE;
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
		m_TopRenderable.AttachChildNode(pSceneGraphNode);
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
		m_TopRenderable.DettachChildNode(pSceneGraphNode);
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

void cView::ProcessControlableList()
{
	if (m_bHide)
		return;

	std::list<IControlable*>::iterator it_control=m_ControlableList.begin();
	for ( ;it_control!=m_ControlableList.end() ; ++it_control )
	{
		(*it_control)->Control();
	}	
}

void cView::ProcessProgressableList(DWORD elapseTime)
{
	std::list<IUpdatable*>::iterator it=m_ProgressableList.begin();
	for ( ;it!=m_ProgressableList.end() ; ++it )
	{
		(*it)->Update(elapseTime);
	}	
}

void cView::ProcessRenderableList()
{
	std::list<IRenderable*>::iterator it=m_RenderableList.begin();
	for ( ;it!=m_RenderableList.end() ; ++it )
	{
		(*it)->ProcessRender();
	}
}

void cView::Update( DWORD elapseTime )
{	
	m_TopRenderable.Update(elapseTime);		// SceneGraph는 계층트리구조 부모우선 순회가 이루어져야한다.
	ProcessProgressableList(elapseTime);	// 다른 인스턴스를 갱신한다.
}

void cView::Control()
{
	ProcessControlableList();
}

void cView::ProcessRender()
{
	if (m_bHide)
		return;

	SetViewPort();
	m_TopRenderable.ProcessRender();		// SceneGraph는 계층트리구조 부모우선 순회가 이루어져야한다.
	ProcessRenderableList();
}

void cView::SetViewPortInfo( UINT x,UINT y,UINT width,UINT height )
{
	m_pD3DDevice->GetViewport(&m_ViewPortInfo);
	m_ViewPortInfo.X=x;
	m_ViewPortInfo.Y=y;
	m_ViewPortInfo.Width=width;
	m_ViewPortInfo.Height=height;	
}

void cView::SetViewPort()
{
	m_pD3DDevice->SetViewport(&m_ViewPortInfo);		
}

