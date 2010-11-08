#pragma once
#include "Foundation/Interface.h"

class cView;
class cDebugInfoView;
class cViewMng:
	public IUnknownObject,
	public IRenderable,
	public IUpdatable,
	public IControlable
{
public:
	cViewMng(void);
	~cViewMng(void);
private:
	// 최상위 배타적인 씬
	cView*			m_pPrevTopScene;
	cView*			m_pCurrTopScene;
	cView*			m_pNewTopScene;

	cDebugInfoView* m_pDebugInfoView;

	IUpdatable* m_pIProgressable;
	IRenderable*	m_pIRednerable;
	IControlable*	m_pIControlable;
	
public:
	void	ChangeTopScene(cView* pScene) { m_pNewTopScene=pScene; }
	cView* GetCurrTopScene() { return m_pCurrTopScene; }
	cView* GetPrevTopScene() { return m_pPrevTopScene; }
	cView* GetNewTopScene() { return m_pNewTopScene; }

	BOOL	IsTopScene(cView* pScene);
	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Control();
};
