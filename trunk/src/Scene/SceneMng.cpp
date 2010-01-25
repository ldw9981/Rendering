
#include "SceneMng.h"
#include "Scene.h"
#include "../Framework/Interface.h"

SceneMng::SceneMng(void)
{
	m_pNewTopScene = NULL;
	m_pCurrTopScene = NULL;
	m_pPrevTopScene = NULL;
	m_pIProgressable = NULL;
	m_pIRednerable = NULL;
	m_pIControlable = NULL;
}

SceneMng::~SceneMng(void)
{
}

void SceneMng::Update( DWORD elapseTime )
{
	// Check m_pNewExclusiveScene
	if (m_pNewTopScene!=NULL)
	{
		// Execute Close
		if (m_pCurrTopScene!=NULL)
			m_pCurrTopScene->Close();

		// update  m_pCurrExclusiveScene
		m_pPrevTopScene	= m_pCurrTopScene;
		m_pCurrTopScene	= m_pNewTopScene;
		m_pNewTopScene	= NULL;
 
		m_pIRednerable = dynamic_cast<Framework::IRenderable*>(m_pCurrTopScene);
		m_pIProgressable = dynamic_cast<Framework::IUpdatable*>(m_pCurrTopScene);
		m_pIControlable = dynamic_cast<Framework::IControlable*>(m_pCurrTopScene);

		// Execute Open
		m_pCurrTopScene->Open(NULL);
	}	

	if (m_pIProgressable!=NULL)
	{
		m_pIProgressable->Update(elapseTime);
	}
		
}

void SceneMng::ProcessRender()
{
	if (m_pIRednerable!=NULL)
	{
		m_pIRednerable->ProcessRender();		
	}	
}

void SceneMng::Control()
{
	if (m_pIControlable!=NULL)
	{
		m_pIControlable->Control();
	}
}

BOOL SceneMng::IsTopScene(Scene* pScene)
{
	if (m_pCurrTopScene != pScene)
		return FALSE;
	
	return TRUE;
}