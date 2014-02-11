#pragma once
#include "Entity.h"
#include "Foundation/Interface.h"
#include "RendererQueue.h"
#include "Scene/CameraNode.h"

class Entity;
class ZTerrain;
class World	:
	public IUnknownObject,
	public IUpdatable,
	public IRenderable
{
public:
	World(void);
	virtual ~World(void);

public:
	
	D3DVIEWPORT9			m_ViewPortInfo;
	// entity
	std::list<Entity*>		m_listEntity;
	std::list<Entity*>		m_listEntityShadow;
	std::list<Entity*>		m_listEntityRender;
	
	cRendererQueue			m_renderQueueNormal[16];
	cRendererQueue			m_renderQueueBlend[16];
	cRendererQueue			m_renderQueueTerrain;
	cRendererQueue			m_renderQueueGUI;

	cRendererQueue			m_renderQueueNormalShadow;
	cRendererQueue			m_renderQueueBlendShadow;
public:
	// camera
	cCameraNode				m_camera;
	// directional light
	D3DXVECTOR4				m_WorldLightPosition;

public:
	// create Entity
	Entity* CreateEntity();
	void DeleteEntity(Entity* pEntity);
	ZTerrain* CreateTerrain( D3DXVECTOR3* pvfScale, const char* lpBMPFilename, const char* lpTexFilename );
	void DeleteTerrain(ZTerrain* pEntity);

	
	// delete Entity

	// find Entity
	virtual void ProcessRender();
	virtual void Update(DWORD elapseTime);
	void CullFrustum();

	D3DVIEWPORT9&			GetViewPortInfo()  { return m_ViewPortInfo; }
	void					SetViewPortInfo(UINT x,UINT y,UINT width,UINT height);
	void					SetViewPortInfo(D3DVIEWPORT9& val) { m_ViewPortInfo = val; }
	void					SetViewPort();

	
};

