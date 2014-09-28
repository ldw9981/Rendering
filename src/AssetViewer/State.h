#pragma once
#include "Framework/View.h"
#include "Scene/SceneNode.h"

class Sophia::Entity;
class State : public Sophia::cView
{
public:
	State(void);
	virtual ~State(void);
private:
	//cCameraNode	m_Camera;
	Sophia::Entity*	m_pTank;
	Sophia::ZTerrain*	m_pZTerrain;
	BOOL		m_bControlCamera;
	Sophia::Entity*			m_pDragon;
	Sophia::Entity*			m_pAirPlaneBake;
	Sophia::Entity*			m_pHouse[10];

	Sophia::Entity*			m_pModel;
	bool					m_bModifiedAnimation;
	bool					m_bModifiedScene;
	bool					m_bModifiedMaterial;	
public:
	Sophia::cSceneNode				m_helper;	
public:	
	virtual void Enter();
	virtual void Leave();

	virtual void Update(DWORD elapseTime);
	virtual void ProcessRender();
	virtual void Control();

	void OpenASE(const char* path);
	void OpenAsset(const char* path);
	void SaveAsset();


	Sophia::Entity* GetEntity();
	bool GetModifiedScene() const { return m_bModifiedScene; }
	void SetModifiedScene(bool val) { m_bModifiedScene = val; }
	bool GetModifiedMaterial() const { return m_bModifiedMaterial; }
	void SetModifiedMaterial(bool val) { m_bModifiedMaterial = val; }
	bool GetModifiedAnimation() const { return m_bModifiedAnimation; }
	void SetModifiedAnimation(bool val) { m_bModifiedAnimation = val; }
};

