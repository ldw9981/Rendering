#pragma once
#include "../Framework/Interface.h"

class Scene;
class SceneMng:
	public Framework::IUnknown,
	public Framework::IRenderable,
	public Framework::IUpdatable,
	public Framework::IControlable
{
public:
	SceneMng(void);
	~SceneMng(void);
private:
	// 최상위 배타적인 씬
	Scene*			m_pPrevTopScene;
	Scene*			m_pCurrTopScene;
	Scene*			m_pNewTopScene;

	Framework::IUpdatable* m_pIProgressable;
	Framework::IRenderable*	m_pIRednerable;
	Framework::IControlable*	m_pIControlable;
	
public:
	void	ChangeTopScene(Scene* pScene) { m_pNewTopScene=pScene; }
	Scene* GetCurrTopScene() { return m_pCurrTopScene; }
	Scene* GetPrevTopScene() { return m_pPrevTopScene; }
	Scene* GetNewTopScene() { return m_pNewTopScene; }

	BOOL	IsTopScene(Scene* pScene);
	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Control();
};
