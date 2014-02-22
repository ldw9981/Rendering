#pragma once
#include "Entity.h"
#include "Foundation/Interface.h"
#include "RendererQueue.h"
#include "Scene/CameraNode.h"

namespace Sophia
{
class Entity;
class ZTerrain;
class cGUIButton;
class cGUIFont;
class World	:
	public IUnknownObject,
	public IUpdatable,
	public IRenderable
{
public:
	World(void);
	virtual ~World(void);

public:
	// �׸��ڸ� ����Ÿ��
	LPDIRECT3DTEXTURE9		m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9		m_pShadowDepthStencil;
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

	std::map<cGUIButton*,cGUIButton*> m_mapButton;
	std::map<cGUIFont*,cGUIFont*> m_mapFont;
public:
	// camera
	cCameraNode				m_camera;
	// directional light
	D3DXVECTOR4				m_WorldLightPosition;
	bool					m_bDebugBound;
protected:
public:
	// create,delete
	Entity* CreateEntity();
	void DeleteEntity(Entity* pEntity);
	ZTerrain* CreateTerrain( D3DXVECTOR3* pvfScale, const char* lpBMPFilename, const char* lpTexFilename );
	void DeleteTerrain(ZTerrain* pEntity);

	cGUIButton* CreateButton( const char* strImageFile);
	void DeleteButton(cGUIButton* pButton);

	cGUIFont* CreateFont();
	void DeleteFont(cGUIFont* pButton);

	virtual void ProcessRender();
	virtual void Update(DWORD elapseTime);
	void CullFrustum();

	void					SetViewPortInfo(UINT x,UINT y,UINT width,UINT height);
	void					SetViewPortInfo(D3DVIEWPORT9& val) { m_ViewPortInfo = val; }	
	bool Initialize();
	void Finalize();
	void Render();
};

}
