#include "StdAfx.h"
#include "SceneMng.h"
#include "Scene.h"
#include "../Common/Interface.h"

cSceneMng::cSceneMng(void)
{
	m_pNewTopScene = NULL;
	m_pCurrTopScene = NULL;
	m_pPrevTopScene = NULL;
	m_pIProgressable = NULL;
	m_pIRednerable = NULL;
	m_pIControlable = NULL;
}

cSceneMng::~cSceneMng(void)
{
}

void cSceneMng::Update( DWORD elapseTime )
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
 
		m_pIRednerable = dynamic_cast<cIRenderable*>(m_pCurrTopScene);
		m_pIProgressable = dynamic_cast<cIUpdatable*>(m_pCurrTopScene);
		m_pIControlable = dynamic_cast<cIControlable*>(m_pCurrTopScene);

		// Execute Open
		m_pCurrTopScene->Open(NULL);
	}	

	if (m_pIProgressable!=NULL)
	{
		m_pIProgressable->Update(elapseTime);
	}
		
}

void cSceneMng::ProcessRender()
{
	if (m_pIRednerable!=NULL)
	{
		m_pIRednerable->ProcessRender();		
	}	
}

void cSceneMng::Control()
{
	if (m_pIControlable!=NULL)
	{
		m_pIControlable->Control();
	}
}

BOOL cSceneMng::IsTopScene(cScene* pScene)
{
	if (m_pCurrTopScene != pScene)
		return FALSE;
	
	return TRUE;
}