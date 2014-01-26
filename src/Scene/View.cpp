#include "StdAfx.h"
#include "View.h"
#include "Scene/CameraNode.h"
#include "Graphics/Graphics.h"
#include "Math/CollisionDetector.h"


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
	// Control�� Scene���� ȣ��
	IControlable* pIC = dynamic_cast< IControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_ControlableList.push_back(pIC);
		pIC->m_ItControlable = --m_ControlableList.end();	
	}

	Entity* pEntity = dynamic_cast<Entity*>(pIUnknownObject);
	if (pEntity !=NULL)
	{
		//m_listEntity.AttachChildNode(pSceneGraphNode);
		m_listEntity.push_back(pEntity);
		pEntity->m_itEntityList = --m_listEntity.end();	
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
	// Control�� Scene���� ȣ��
	IControlable* pIC = dynamic_cast< IControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_ControlableList.erase(pIC->m_ItControlable);
	}	

	Entity* pEntity = dynamic_cast<Entity*>(pIUnknownObject);
	if (pEntity !=NULL)
	{
		m_listEntity.erase(pEntity->m_itEntityList);
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
	auto itEntity = m_listEntity.begin();
	for ( ;itEntity!=m_listEntity.end() ; ++itEntity )
	{
		(*itEntity)->Update(elapseTime);
	}	

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
	CullFrustum();
	
	Graphics::g_pGraphics->Render(this);	

	m_listEntityShadow.clear();
	m_listEntityRender.clear();


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
	Graphics::m_pDevice->SetViewport(&m_ViewPortInfo);				
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

void cView::DebugRender()
{
	auto itEntity = m_listEntity.begin();
	for ( ;itEntity!=m_listEntity.end() ; ++itEntity )
	{
		(*itEntity)->RenderBound();
	}	
}

void cView::CullFrustum()
{
	cCameraNode* pActiveCamera = cCameraNode::GetActiveCamera();
	if (!pActiveCamera)
		return;

	Frustum& frustum = pActiveCamera->GetFrustum();

	for ( auto itIn = m_listEntity.begin() ;itIn!=m_listEntity.end() ; ++itIn )
	{
		if( (*itIn)->CullRendererIntoRendererQueue(&frustum,500.0f) == false )
			continue;	
		

		m_listEntityShadow.push_back(*itIn);

		if( (*itIn)->CullRendererIntoRendererQueue(&frustum,0.0f) == false )
			continue;	

		m_listEntityRender.push_back(*itIn);
	}
}

void cView::ProcessRenderEx()
{
	if (m_bHide)
		return;

	// scene 
	//CullFrustum(m_listEntity,m_listEntityShadow,100.0f);
	//CullFrustum(m_listEntityShadow,m_listEntityRender,0.0f);

	cCameraNode* pActiveCamera = cCameraNode::GetActiveCamera();
	if (pActiveCamera)
	{
		Frustum& frustum = pActiveCamera->GetFrustum();

		cCollision::STATE retCS;
		for ( auto itIn = m_listEntity.begin() ;itIn!=m_listEntity.end() ; ++itIn )
		{
			retCS = cCollision::CheckWorldFrustumWithoutYAxis(frustum, (*itIn)->GetBoundingSphere() ,500.0f);
			if( retCS == cCollision::OUTSIDE)
				continue;

			// gather shadow Render
			m_renderQueueNormalShadow.Insert((*itIn)->m_renderQueueNormalShadow);
			m_renderQueueBlendShadow.Insert((*itIn)->m_renderQueueBlendShadow);

			retCS = cCollision::CheckWorldFrustum(frustum, (*itIn)->GetBoundingSphere());
			if( retCS == cCollision::OUTSIDE)
				continue;

			// gather model Render
			for(int i=0;i<16;i++)
			{
				m_renderQueueNormal[i].Insert((*itIn)->m_renderQueueNormal[i]);
				m_renderQueueBlend[i].Insert((*itIn)->m_renderQueueBlend[i]);
			}	
		}
	}

	Graphics::g_pGraphics->Render(this);	

	m_renderQueueNormalShadow.Clear();
	m_renderQueueBlendShadow.Clear();
	for(int i=0;i<16;i++)
	{
		m_renderQueueNormal[i].Clear();
		m_renderQueueBlend[i].Clear();
	}

	// gui 
	std::list<IRenderable*>::iterator it=m_RenderableList.begin();
	for ( ;it!=m_RenderableList.end() ; ++it )
	{
		(*it)->ProcessRender();	
	}

	// state 
	m_ViewState.ProcessRender();
}
