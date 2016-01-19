#pragma once
#include "Entity.h"
#include "Foundation/Interface.h"
#include "RendererQueue.h"
#include "Graphics/CameraNode.h"
#include "Graphics.h"

namespace Sophia
{
class Entity;
class ZTerrain;
class cGUIButton;
class cGUIFont;
class cRscTexture;
class World	:
	public IUnknownObject,
	public IUpdatable,
	public IRenderable
{
public:
	World(void);
	virtual ~World(void);
	typedef std::pair<Entity*,float> VISIBILITY_ENTITY;
	typedef boost::fast_pool_allocator<VISIBILITY_ENTITY,boost::default_user_allocator_new_delete, boost::details::pool::null_mutex> VISIBILITY_ENTITY_ALLOCATOR ;
public:
	LPDIRECT3DTEXTURE9		m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9		m_pShadowDepthStencil;
	D3DVIEWPORT9			m_ViewPortInfo;

	// entity
	std::list<Entity*>		m_listEntity;
	

	std::list<VISIBILITY_ENTITY,VISIBILITY_ENTITY_ALLOCATOR>		m_listEntityRender;
	
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

	LPDIRECT3DSURFACE9 m_pHWRenderTarget[4];
	LPDIRECT3DSURFACE9 m_pHWDepthStencilBuffer;
public:
	// camera
	cCameraNode				m_camera;
	// directional light
	D3DXVECTOR3				m_worldLightLookAt;	
	D3DXVECTOR3				m_worldLightDirection;
	bool					m_bDebugBound;
	bool					m_bEnableShadow;
protected:
	D3DXVECTOR3				m_worldLightPosition;
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

	void RenderShadow();
	void RenderScene();

	static bool LessDistance(VISIBILITY_ENTITY& a,VISIBILITY_ENTITY& b);
	D3DXVECTOR3 GetWorldLightDirection() const { return m_worldLightDirection; }
	void SetWorldLightDirection(const D3DXVECTOR3& val);
};

}
