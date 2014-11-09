#pragma once
#include "Foundation/Interface.h"
namespace Sophia
{

class FiniteState
{
public:
	FiniteState(void);
	~FiniteState(void);
public:
	virtual void Enter()=0;
	virtual void Leave()=0;
	virtual void Update(DWORD elapseTime)=0;
};

template <typename T>
class FiniteStateMachine
{
public:
	FiniteStateMachine(void);
	~FiniteStateMachine(void);
protected:
	T*			m_pPrev;
	T*			m_pCurr;
	T*			m_pNew;
public:
	void Transite( T* pState)
	{
		m_pNew = pState;
	}
	void Update( DWORD elapseTime )
	{
		if (m_pNew!=NULL)
		{		
			if (m_pCurr!=NULL)
				m_pCurr->Leave();

			m_pPrev	= m_pCurr;
			m_pCurr	= m_pNew;
			m_pNew	= NULL;

			// Execute Open
			m_pCurr->Enter();
		}	

		if (m_pCurr!=NULL)
		{
			m_pCurr->Update(elapseTime);
		}
	}

	bool IsCurr( T* pState )
	{
		if( pState != m_pCurr )
			return false;
		return true;
	}	

	T* GetCurr()
	{
		return m_pCurr;
	}
};



class cView;
class cDebugInfoView;
class cViewMng:
	public IUnknownObject,
	public IRenderable,
	public IRenderer,
	public IUpdatable,	
	public IControlable,
	public FiniteStateMachine<cView>
{
public:
	cViewMng(void);
	~cViewMng(void);

public:
	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Render();
	virtual void Control();
};
}