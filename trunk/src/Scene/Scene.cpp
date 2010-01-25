
#include "Scene.h"
#include "TopRenderable.h"
#include "../D3D9/SharedDevice.h"
#include "../Scene/CameraNode.h"

Scene::Scene(void)
:m_pParentScene(NULL)
{
	m_bHide=FALSE;
}

Scene::Scene( Scene* pParent )
:m_pParentScene(pParent)
{
	m_bHide=FALSE;
}
Scene::~Scene(void)
{
}

void Scene::AttachObject( Framework::IUnknown* pIUnknownObject )
{
	// Control�� Scene���� ȣ��
	Framework::IControlable* pIC = dynamic_cast< Framework::IControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_ControlableList.push_back(pIC);
		pIC->m_ItControlable = --m_ControlableList.end();	
	}

	SceneGraphNode* pSceneGraphNode = dynamic_cast<SceneGraphNode*>(pIUnknownObject);
	if (pSceneGraphNode !=NULL)
	{
		m_TopRenderable.AttachChildNode(pSceneGraphNode);
		return;
	}


	Framework::IRenderable* pIR = dynamic_cast<Framework::IRenderable*>(pIUnknownObject);
	if(pIR != NULL)
	{
		m_RenderableList.push_back(pIR);
		pIR->m_ItRenderable = --m_RenderableList.end();	
	}

	Framework::IUpdatable* pIP = dynamic_cast< Framework::IUpdatable *>(pIUnknownObject);
	if(pIP != NULL)
	{
		m_ProgressableList.push_back(pIP);
		pIP->m_ItProgressable = --m_ProgressableList.end();	
	}


}

void Scene::DettachObject( Framework::IUnknown* pIUnknownObject )
{
	// Control�� Scene���� ȣ��
	Framework::IControlable* pIC = dynamic_cast< Framework::IControlable *>(pIUnknownObject);
	if(pIC != NULL)
	{
		m_ControlableList.erase(pIC->m_ItControlable);
	}	

	SceneGraphNode* pSceneGraphNode = dynamic_cast<SceneGraphNode*>(pIUnknownObject);
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

void Scene::ProcessControlableList()
{
	if (m_bHide)
		return;

	list<Framework::IControlable*>::iterator it_control=m_ControlableList.begin();
	for ( ;it_control!=m_ControlableList.end() ; ++it_control )
	{
		(*it_control)->Control();
	}	
}

void Scene::ProcessProgressableList(DWORD elapseTime)
{
	list<Framework::IUpdatable*>::iterator it=m_ProgressableList.begin();
	for ( ;it!=m_ProgressableList.end() ; ++it )
	{
		(*it)->Update(elapseTime);
	}	
}

void Scene::ProcessRenderableList()
{
	list<Framework::IRenderable*>::iterator it=m_RenderableList.begin();
	for ( ;it!=m_RenderableList.end() ; ++it )
	{
		(*it)->ProcessRender();
	}
}

void Scene::Update( DWORD elapseTime )
{	
	m_TopRenderable.Update(elapseTime);		// SceneGraph�� ����Ʈ������ �θ�켱 ��ȸ�� �̷�������Ѵ�.
	ProcessProgressableList(elapseTime);	// �ٸ� �ν��Ͻ��� �����Ѵ�.
}

void Scene::Control()
{
	ProcessControlableList();
}

void Scene::ProcessRender()
{
	if (m_bHide)
		return;

	SetViewPort();
	m_TopRenderable.ProcessRender();		// SceneGraph�� ����Ʈ������ �θ�켱 ��ȸ�� �̷�������Ѵ�.
	ProcessRenderableList();
}

void Scene::SetViewPortInfo( UINT x,UINT y,UINT width,UINT height )
{
	m_SharedLPDIRECT3DDEVICE9->GetViewport(&m_ViewPortInfo);
	m_ViewPortInfo.X=x;
	m_ViewPortInfo.Y=y;
	m_ViewPortInfo.Width=width;
	m_ViewPortInfo.Height=height;	
}

void Scene::SetViewPort()
{
	m_SharedLPDIRECT3DDEVICE9->SetViewport(&m_ViewPortInfo);		
}

