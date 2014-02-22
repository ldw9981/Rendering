#include "StdAfx.h"
#include "ViewMng.h"
#include "View.h"
#include "Foundation/Interface.h"
#include "DebugInfoView.h"
#include "Foundation/Define.h"


namespace Sophia
{

FiniteState::FiniteState( void )
{

}

FiniteState::~FiniteState( void )
{

}

template <typename T>
FiniteStateMachine<T>::FiniteStateMachine( void )
{
	m_pPrev = NULL;
	m_pCurr = NULL;
	m_pNew = NULL;
}

template <typename T>
FiniteStateMachine<T>::~FiniteStateMachine( void )
{

}







cViewMng::cViewMng(void)
{

}

cViewMng::~cViewMng(void)
{
	//SAFE_DELETE(m_pDebugInfoView);
}

void cViewMng::ProcessRender()
{
	if (m_pCurr)
	{
		m_pCurr->ProcessRender();
	}
}

void cViewMng::Control()
{
	if (m_pCurr)
	{
		m_pCurr->Control();
	}
}



void cViewMng::Render()
{
	if (m_pCurr)
	{
		m_pCurr->Render();
	}
}

void cViewMng::Update( DWORD elapseTime )
{
	FiniteStateMachine::Update(elapseTime);
}


}