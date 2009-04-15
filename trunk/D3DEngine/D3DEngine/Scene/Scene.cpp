#include "StdAfx.h"
#include "Scene.h"
#include "TopRenderable.h"
#include "../Framework/StaticD3DDevice9.h"
#include "../SceneGraph/CameraNode.h"

cScene::cScene(void)
:m_pParentScene(NULL)
{
	m_bHide=FALSE;
}

cScene::cScene( cScene* pParent )
:m_pParentScene(pParent)
{
	m_bHide=FALSE;
}
cScene::~cScene(void)
{
}

void cScene::AttachObject( IUnknownObject* pIUnknownObject )
{
	// Control은 Scene에서 호출
	IControlable* pIC = dynamic_cast< IControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_ControlableList.push_back(pIC);
		pIC->m_ItControlable = --m_ControlableList.end();	
	}

	cSceneGraphNode* pSceneGraphNode = dynamic_cast<cSceneGraphNode*>(pIUnknownObject);
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

void cScene::DettachObject( IUnknownObject* pIUnknownObject )
{
	// Control은 Scene에서 호출
	IControlable* pIC = dynamic_cast< IControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_ControlableList.erase(pIC->m_ItControlable);
	}	

	cSceneGraphNode* pSceneGraphNode = dynamic_cast<cSceneGraphNode*>(pIUnknownObject);
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

void cScene::ProcessControlableList()
{
	if (m_bHide)
		return;

	list<IControlable*>::iterator it_control=m_ControlableList.begin();
	for ( ;it_control!=m_ControlableList.end() ; ++it_control )
	{
		(*it_control)->Control();
	}	
}

void cScene::ProcessProgressableList(DWORD elapseTime)
{
	list<IUpdatable*>::iterator it=m_ProgressableList.begin();
	for ( ;it!=m_ProgressableList.end() ; ++it )
	{
		(*it)->Update(elapseTime);
	}	
}

void cScene::ProcessRenderableList()
{
	list<IRenderable*>::iterator it=m_RenderableList.begin();
	for ( ;it!=m_RenderableList.end() ; ++it )
	{
		(*it)->ProcessRender();
	}
}

void cScene::Update( DWORD elapseTime )
{	
	m_TopRenderable.Update(elapseTime);		// SceneGraph는 계층트리구조 부모우선 순회가 이루어져야한다.
	ProcessProgressableList(elapseTime);	// 다른 인스턴스를 갱신한다.
}

void cScene::Control()
{
	ProcessControlableList();
}

void cScene::ProcessRender()
{
	if (m_bHide)
		return;

	SetViewPort();
	m_TopRenderable.ProcessRender();		// SceneGraph는 계층트리구조 부모우선 순회가 이루어져야한다.
	ProcessRenderableList();
}

void cScene::SetViewPortInfo( UINT x,UINT y,UINT width,UINT height )
{
	m_pD3DDevice->GetViewport(&m_ViewPortInfo);
	m_ViewPortInfo.X=x;
	m_ViewPortInfo.Y=y;
	m_ViewPortInfo.Width=width;
	m_ViewPortInfo.Height=height;	
}

void cScene::SetViewPort()
{
	m_pD3DDevice->SetViewport(&m_ViewPortInfo);		
}

