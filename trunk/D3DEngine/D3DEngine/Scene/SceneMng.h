#pragma once
#include "../Common/Interface.h"

class cScene;
class cSceneMng:
	public IUnknownObject,
	public IRenderable,
	public IUpdatable,
	public IControlable
{
public:
	cSceneMng(void);
	~cSceneMng(void);
private:
	// 최상위 배타적인 씬
	cScene*			m_pPrevTopScene;
	cScene*			m_pCurrTopScene;
	cScene*			m_pNewTopScene;

	IUpdatable* m_pIProgressable;
	IRenderable*	m_pIRednerable;
	IControlable*	m_pIControlable;
	
public:
	void	ChangeTopScene(cScene* pScene) { m_pNewTopScene=pScene; }
	cScene* GetCurrTopScene() { return m_pCurrTopScene; }
	cScene* GetPrevTopScene() { return m_pPrevTopScene; }
	cScene* GetNewTopScene() { return m_pNewTopScene; }

	BOOL	IsTopScene(cScene* pScene);
	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Control();
};
