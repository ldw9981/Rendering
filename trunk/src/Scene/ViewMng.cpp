#include "StdAfx.h"
#include "ViewMng.h"
#include "View.h"
#include "Foundation/Interface.h"
#include "DebugInfoView.h"
#include "Foundation/Define.h"
#include "Scene/DebugInfoView.h"

cViewMng::cViewMng(void)
{
	m_pNewTopScene = NULL;
	m_pCurrTopScene = NULL;
	m_pPrevTopScene = NULL;
	m_pIProgressable = NULL;
	m_pIRednerable = NULL;
	m_pIControlable = NULL;
	m_pDebugInfoView = new cDebugInfoView;
}

cViewMng::~cViewMng(void)
{
	SAFE_DELETE(m_pDebugInfoView);
}

void cViewMng::Update( DWORD elapseTime )
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
 
		m_pIRednerable = dynamic_cast<IRenderable*>(m_pCurrTopScene);
		m_pIProgressable = dynamic_cast<IUpdatable*>(m_pCurrTopScene);
		m_pIControlable = dynamic_cast<IControlable*>(m_pCurrTopScene);

		// Execute Open
		m_pCurrTopScene->Open(NULL);
	}	

	if (m_pIProgressable!=NULL)
	{
		m_pIProgressable->Update(elapseTime);

		
	}
		
}

void cViewMng::ProcessRender()
{
	if (m_pIRednerable!=NULL)
	{
		m_pIRednerable->ProcessRender();		

	}	
}

void cViewMng::Control()
{
	if (m_pIControlable!=NULL)
	{
		m_pIControlable->Control();
		
	}
}

BOOL cViewMng::IsTopScene(cView* pScene)
{
	if (m_pCurrTopScene != pScene)
		return FALSE;
	
	return TRUE;
}