#include "StdAfx.h"
#include "View.h"
#include "Graphics/CameraNode.h"
#include "Graphics/Graphics.h"
#include "Math/CollisionDetector.h"
#include "Graphics/World.h"
namespace Sophia
{


cView::cView(void)
:m_pParentView(NULL)
{
	m_bHide=FALSE;
	AttachObject(&m_graphicWorld);
}

cView::~cView(void)
{
	DettachObject(&m_graphicWorld);
}

void cView::AttachObject( IUnknownObject* pIUnknownObject )
{
	// Control�� Scene���� ȣ��
	IControlable* pIC = dynamic_cast< IControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_ControlableList.push_back(pIC);
		pIC->m_ItControlable = --m_ControlableList.end();	
	}
	/*
	Entity* pEntity = dynamic_cast<Entity*>(pIUnknownObject);
	if (pEntity !=NULL)
	{
		//m_listEntity.AttachChildNode(pSceneGraphNode);
		m_listEntity.push_back(pEntity);
		pEntity->m_itEntityList = --m_listEntity.end();	
		return;
	}
	*/

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
	// Control�� Scene���� ȣ��
	IControlable* pIC = dynamic_cast< IControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_ControlableList.erase(pIC->m_ItControlable);
	}	

	/*
	Entity* pEntity = dynamic_cast<Entity*>(pIUnknownObject);
	if (pEntity !=NULL)
	{
		m_listEntity.erase(pEntity->m_itEntityList);
		return;
	}
	*/

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
	/*
	auto itEntity = m_listEntity.begin();
	for ( ;itEntity!=m_listEntity.end() ; ++itEntity )
	{
		(*itEntity)->Update(elapseTime);
	}	
	*/
	std::list<IUpdatable*>::iterator it=m_ProgressableList.begin();
	for ( ;it!=m_ProgressableList.end() ; ++it )
	{
		(*it)->Update(elapseTime);
	}		
	m_ViewState.Update(elapseTime);
}

void cView::Control(DWORD elapseTime)
{
	if (m_bHide)
		return;

	std::list<IControlable*>::iterator it_control=m_ControlableList.begin();
	for ( ;it_control!=m_ControlableList.end() ; ++it_control )
	{
		(*it_control)->Control(elapseTime);
	}	
	m_ViewState.Control(elapseTime);
}

void cView::ProcessRender(DWORD elapseTime)
{
	if (m_bHide)
		return;
	
	// scene 
	/*
	CullFrustum();	
	Graphics::g_pGraphics->Render(this);	
	m_listEntityShadow.clear();
	m_listEntityRender.clear();
	*/

	// gui 
	std::list<IRenderable*>::iterator it=m_RenderableList.begin();
	for ( ;it!=m_RenderableList.end() ; ++it )
	{
		(*it)->ProcessRender(elapseTime);	
	}
	
	// state 
	m_ViewState.ProcessRender(elapseTime);
}



void cView::Render(DWORD elapseTime)
{

}

void cView::Enter()
{
	m_graphicWorld.Initialize();
}

void cView::Leave()
{
	m_graphicWorld.Finalize();
}


void cView::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{

}
}


