#pragma once
#include "Foundation/Interface.h"

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
	std::map<std::string,T*> m_container;
	T*			m_pPrev;
	T*			m_pCurr;
	T*			m_pNew;
public:
	bool Transite( std::string& strName )
	{
		std::map<std::string,T*>::iterator it = m_container.find(strName);
		if (it == m_container.end())
			return false;

		m_pNew = it->second;
		return true;
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
	bool Add( T* pFiniteState,std::string& strName )
	{
		std::map<std::string,T*>::iterator it = m_container.find(strName);
		if (it != m_container.end())
			return false;

		m_container.insert(std::pair<std::string,T*>(strName,pFiniteState));
		return true;
	}
	bool Del( std::string& strName )
	{
		return false;
	}
	bool IsCurr( std::string& strName )
	{
		std::map<std::string,T*>::iterator it = m_container.find(strName);
		if (it == m_container.end())
			return false;

		if( it->second != m_pCurr )
			return false;

		return true;
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
