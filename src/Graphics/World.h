#pragma once
#include "Entity.h"
#include "Foundation/Interface.h"
#include "RendererQueue.h"
#include "Scene/CameraNode.h"
#include "Graphics.h"

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
	// ±×¸²ÀÚ¸Ê ·»´õÅ¸±ê
	LPDIRECT3DTEXTURE9		m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9		m_pShadowDepthStencil;
	D3DVIEWPORT9			m_ViewPortInfo;

	// entity
	std::list<Entity*>		m_listEntity;

	std::list<Entity*>		m_listEntityRender;
	
	cRendererQueue			m_renderQueueNormalShadow;	
	cRendererQueue			m_renderQueueSkinnedShadow;

	cRendererQueue			m_renderQueueNormal;					// sort by material
	cRendererQueue			m_renderQueueSkinned;				// sort by material
	cRendererQueue			m_renderQueueNormalAlphaBlend;	// sort by dist
	cRendererQueue			m_renderQueueSkinnedAlphaBlend;	// sort by dist

	cRendererQueue			m_renderQueueTerrain;
	cRendererQueue			m_renderQueueGUI;

	std::map<cGUIButton*,cGUIButton*> m_mapButton;
	std::map<cGUIFont*,cGUIFont*> m_mapFont;
public:
	// camera
	cCameraNode				m_camera;
	// directional light
	D3DXVECTOR4				m_WorldLightPosition;
	bool					m_bDebugBound;
	bool					m_bEnableShadow;
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
	void GatherRender();

	void					SetViewPortInfo(UINT x,UINT y,UINT width,UINT height);
	void					SetViewPortInfo(D3DVIEWPORT9& val) { m_ViewPortInfo = val; }	
	bool Initialize();
	void Finalize();
	void Render();
	bool GetEnableShadow() const { return m_bEnableShadow; }
	void SetEnableShadow(bool val) { m_bEnableShadow = val; }
};

}
