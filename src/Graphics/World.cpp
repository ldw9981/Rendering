#include "StdAfx.h"
#include "World.h"
#include "GUI/GUIButton.h"
#include "GUI/GUIFont.h"
#include "Graphics.h"
#include "Graphics/CameraNode.h"
#include "Graphics/ZTerrain.h"
#include "Foundation/Define.h"
#include "Graphics/RscTexture.h"
#include "Foundation/Trace.h"
#include "Graphics/Entity.h"
#include "Math/CollisionDetector.h"
#include <algorithm>
#include <execution>


namespace Sophia
{

#define PI           3.14159265f
#define FOV          (PI/4.0f)							// 시야각
#define ASPECT_RATIO (1024/(float)768)		// 화면의 종횡비


#define T_SIZE 256

#define SHADOWMAP_SIZE 4096

bool World::LessDistance(VISIBILITY_ENTITY& a,VISIBILITY_ENTITY& b)
{
	if ( a.second < b.second)
		return true;
	return false;
}

World::World(void)
{
	m_ViewPortInfo.MinZ = 0.0f;
	m_ViewPortInfo.MaxZ = 1.0f;
	m_ViewPortInfo.X = 0;
	m_ViewPortInfo.Y = 0;
	m_ViewPortInfo.Width = Graphics::m_pInstance->m_width;
	m_ViewPortInfo.Height = Graphics::m_pInstance->m_height;
	SetWorldLightDirection(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	m_bDebugBound = false;
	m_bDebugShadow = false;
	m_bEnableShadow = true;
	m_pHWRenderTarget[0] = NULL;
	m_pHWDepthStencilBuffer = NULL;

}


World::~World(void)
{
	for (auto it = m_listEntity.begin() ;  it != m_listEntity.end() ; ++it)
	{
		Entity* pEntity = *it;
		delete pEntity;
	}
	m_listEntity.clear();
}

Entity* World::CreateEntity()
{
	Entity* pEntity = new Entity();	
	AttachEntity(pEntity);
	return pEntity;
}

void World::AttachEntity(Entity * pEntity)
{
	m_listEntity.push_back(pEntity);
	pEntity->m_itEntityList = --m_listEntity.end();
}

void World::DettachEntity(Entity * pEntity)
{
	m_listEntity.erase(pEntity->m_itEntityList);
	pEntity->m_itEntityList = m_listEntity.end();
}

void World::ProcessRender(DWORD elapseTime)
{
	m_camera.Render(elapseTime);
	Render(elapseTime);
}

void World::Update( DWORD elapseTime )
{
	m_camera.Update(elapseTime);

	// 한장의 텍스쳐로 커버
	D3DXVECTOR3 pos;
	float dist = m_camera.GetNear();
	m_camera.GetWorldPosition(pos);
	pos += *m_camera.GetForward() * dist;
	m_worldLightLookAt = pos;	

	pos += m_worldLightDirection * dist * -1.0f;
	m_worldLightPosition = pos;


	m_listEntityHasAlphaRender.clear();
	Frustum& frustum = m_camera.GetFrustum();
	float distFromNear;

	// Animation,WorldMatrix갱신 병렬처리
	
	std::for_each(std::execution::par_unseq, std::begin(m_listEntity), std::end(m_listEntity), [&](Entity* pEntity) {
		pEntity->Update(elapseTime);
	});
	/*
	for (auto itIn = m_listEntity.begin(); itIn != m_listEntity.end(); ++itIn)
	{
		Entity* pEntity = *itIn;
		pEntity->Update(elapseTime);
	}
	*/
	m_renderQueueNormalShadow.Clear();
	m_renderQueueSkinnedShadow.Clear();
	m_renderQueueNormal.Clear();
	m_renderQueueSkinned.Clear();
	m_renderQueueNormalAlphaBlend.Clear();
	m_renderQueueSkinnedAlphaBlend.Clear();
	m_listEntityHasAlphaRender.clear();

	for ( auto itIn = m_listEntity.begin() ;itIn!=m_listEntity.end() ; ++itIn )
	{
		Entity* pEntity = *itIn;
		if (!pEntity->GetShow())
			continue;

		cCollision::STATE retCS = cCollision::CheckWorldFrustum(frustum, pEntity->GetBoundingSphere(), &distFromNear, 0.0f);
		if (retCS == cCollision::OUTSIDE)
			continue;


		if (m_bEnableShadow)
		{
			// 그림자용은 알파블렌드 상관없이 모은다.그릴때는 알파테스트만 구분한다.
			m_renderQueueNormalShadow.GatherRender(pEntity->m_vecNormal);
			m_renderQueueNormalShadow.GatherRender(pEntity->m_vecNormalAlphaBlend);

			m_renderQueueSkinnedShadow.GatherRender(pEntity->m_vecSkinned);
			m_renderQueueSkinnedShadow.GatherRender(pEntity->m_vecSkinnedAlphaBlend);
		}


		// 씬은 알파블렌드로 구분하여 모은다.
		if(!pEntity->m_vecNormal.empty())
			m_renderQueueNormal.GatherRender(pEntity->m_vecNormal);	// normal instancing 분리

		if (!pEntity->m_vecSkinned.empty())
			m_renderQueueSkinned.GatherRender(pEntity->m_vecSkinned);

		// 이거 수정해야함 노드별로 거리계산해서 소팅해야 정확함
		if (!pEntity->m_vecNormalAlphaBlend.empty() || !pEntity->m_vecSkinnedAlphaBlend.empty())
			m_listEntityHasAlphaRender.push_back(std::make_pair(pEntity, distFromNear));
	}

	m_listEntityHasAlphaRender.sort(&World::LessDistance);
	for (auto itIn = m_listEntityHasAlphaRender.begin(); itIn != m_listEntityHasAlphaRender.end(); ++itIn)
	{
		Entity* pEntity = itIn->first;
	
		if (!pEntity->m_vecNormalAlphaBlend.empty())
			m_renderQueueNormalAlphaBlend.GatherRenderAlphaBlend(pEntity->m_vecNormalAlphaBlend, m_camera.GetWorldPositionPtr());

		if (!pEntity->m_vecSkinnedAlphaBlend.empty())
			m_renderQueueSkinnedAlphaBlend.GatherRenderAlphaBlend(pEntity->m_vecSkinnedAlphaBlend, m_camera.GetWorldPositionPtr());
	}


	for ( auto itIn = m_mapButton.begin() ;itIn!=m_mapButton.end() ; ++itIn )
	{
		(*itIn).second->Update(elapseTime);
	}

	
}

void World::SetViewPortInfo(UINT x,UINT y,UINT width,UINT height )
{
	m_ViewPortInfo.X = x;
	m_ViewPortInfo.Y = y;
	m_ViewPortInfo.Width=width;
	m_ViewPortInfo.Height=height;	
}

void World::DeleteEntity( Entity* pEntity )
{
	DettachEntity(pEntity);
	delete pEntity;
}

ZTerrain* World::CreateTerrain( D3DXVECTOR3* pvfScale, const char* lpBMPFilename, const char* lpTexFilename )
{
	ZTerrain* pEntity = new ZTerrain();	
	pEntity->Create(pvfScale,lpBMPFilename,lpTexFilename);
	AttachEntity(pEntity);
	return pEntity;
}

void World::DeleteTerrain( ZTerrain* pEntity )
{
	DettachEntity(pEntity);
	delete pEntity;
}

cGUIButton* World::CreateButton( const char* strImageFile)
{
	cGUIButton* pButton = new cGUIButton;
	pButton->Create(strImageFile);
	pButton->SetWorld(this);
	m_mapButton.insert(std::make_pair(pButton,pButton));
	return pButton;
}

void World::DeleteButton( cGUIButton* pButton )
{
	m_mapButton.erase(pButton);
	delete pButton;
}

cGUIFont* World::CreateFont()
{
	cGUIFont* pFont = new cGUIFont;
	pFont->Create();
	pFont->SetWorld(this);
	m_mapFont.insert(std::make_pair(pFont,pFont));
	return pFont;
}

void World::DeleteFont( cGUIFont* pFont )
{
	m_mapFont.erase(pFont);
	delete pFont;
}

bool World::Initialize()
{
	HRESULT hr;
	HR_V( Graphics::m_pDevice->SetViewport(&m_ViewPortInfo));

	// 렌더타깃을 만든다.
	const int shadowMapSize = SHADOWMAP_SIZE;
	HR_V( Graphics::m_pDevice->CreateTexture( shadowMapSize, shadowMapSize,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL) );
		
	// 그림자 맵과 동일한 크기의 깊이버퍼도 만들어줘야 한다.
	HR_V( Graphics::m_pDevice->CreateDepthStencilSurface(shadowMapSize, shadowMapSize,
		D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE,
		&m_pShadowDepthStencil, NULL) );

	return true;
}

void World::Finalize()
{
	SAFE_RELEASE(m_pShadowDepthStencil);
	SAFE_RELEASE(m_pShadowRenderTarget);
}

void World::Render(DWORD elapseTime)
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();	
	UINT passes = 0;
	D3DXMATRIX matLightView;
	{	
		D3DXVECTOR3 vUpVec(    0.0f, 1.0f,  0.0f );
		D3DXMatrixLookAtLH( &matLightView, &m_worldLightPosition, &m_worldLightLookAt, &vUpVec );
	}
	D3DXMATRIX matLightProjection;
	{		
		//D3DXMatrixPerspectiveFovLH( &matLightProjection, m_camera.GetFOV(), 1, 3000, m_camera.GetFar() );
		D3DXMatrixOrthoLH( &matLightProjection, SHADOWMAP_SIZE,SHADOWMAP_SIZE, 1,  m_camera.GetFar());
	}

	Graphics::m_pInstance->SetEffectVector_WorldLightDirection(&D3DXVECTOR4(m_worldLightDirection.x,m_worldLightDirection.y,m_worldLightDirection.z,0.0f));
	Graphics::m_pInstance->SetEffectMatirx_LightView(&matLightView);
	Graphics::m_pInstance->SetEffectMatirx_LightProjection(&matLightProjection);

	RenderShadow(elapseTime);		
	RenderScene(elapseTime);

	if (m_bDebugBound)
	{
		if (!m_listEntityHasAlphaRender.empty())
		{
			pEffect->SetTechnique(Graphics::m_pInstance->m_hTLine);
			pEffect->Begin(&passes, 0);	
			pEffect->BeginPass(0);
			auto itEntityRender = m_listEntityHasAlphaRender.begin();
			for ( ;itEntityRender != m_listEntityHasAlphaRender.end() ; ++itEntityRender )
			{
				VISIBILITY_ENTITY& item = *itEntityRender;
				item.first->RenderBound();
			}	
			pEffect->EndPass();
			pEffect->End();	
		}
	}

	if (!m_mapButton.empty())
	{
		pEffect->SetTechnique(Graphics::m_pInstance->m_hTGUI);
		pEffect->Begin(&passes, 0);	
		pEffect->BeginPass(0);
		for ( auto it = m_mapButton.begin();it != m_mapButton.end() ; ++it )
		{
			(*it).second->Render(elapseTime);
		}	
		pEffect->EndPass();
		pEffect->End();
	}	
	// SHADOW_MAP		
	if (m_bDebugShadow)
	{
		Graphics::m_pDevice->SetTexture (0, m_pShadowRenderTarget );
		Graphics::m_pDevice->SetFVF(FVF_GUIVERTEX);
		pEffect->CommitChanges();
		Graphics::m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, & Graphics::m_pInstance->g_vertices[0], sizeof(GUIVERTEX));	
	}	
}

void World::RenderShadow(DWORD elapseTime)
{
	// 백업후 렌더타켓,스텐실버퍼 변경
	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
	Graphics::m_pDevice->GetRenderTarget(0, &pHWBackBuffer);
	Graphics::m_pDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);	
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if( SUCCEEDED( m_pShadowRenderTarget->GetSurfaceLevel( 0, &pShadowSurface ) ) )
	{
		Graphics::m_pDevice->SetRenderTarget( 0, pShadowSurface );
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	Graphics::m_pDevice->SetDepthStencilSurface( m_pShadowDepthStencil );
	Graphics::m_pDevice->Clear( 0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0 );

	if (m_bEnableShadow)
	{
		if (!m_renderQueueNormalShadow.m_materialOrder.empty())
		{	
			Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pNormalVertexDeclaration);
			m_renderQueueNormalShadow.RenderShadowByMaterialOrder(elapseTime,Graphics::m_pInstance->m_hTShadowNormal,
				Graphics::m_pInstance->m_hTShadowNormalAlphaTest );
		}			


		if (!m_renderQueueSkinnedShadow.m_materialOrder.empty())
		{	
			Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pSkinnedVertexDeclaration);
			m_renderQueueSkinnedShadow.RenderShadowByMaterialOrder(elapseTime,Graphics::m_pInstance->m_hTShadowSkinned,
				Graphics::m_pInstance->m_hTShadowSkinnedAlphaTest );
		}			

		if (!m_renderQueueNormalShadow.m_sceneOrder.empty())
		{
			m_renderQueueNormalShadow.RenderShadowNormalInstancing(elapseTime,Graphics::m_pInstance->m_hTShadowNormalInstancing,
				Graphics::m_pInstance->m_hTShadowNormalInstancingAlphaTest );		
		}	

		if (!m_renderQueueSkinnedShadow.m_sceneOrder.empty())
		{
			m_renderQueueSkinnedShadow.RenderShadowSkinnedInstancing(elapseTime,Graphics::m_pInstance->m_hTShadowSkinnedInstancing,
				Graphics::m_pInstance->m_hTShadowSkinnedInstancingAlphaTest );		
		}	
	}	
	//////////////////////////////
	// 2. 그림자 입히기
	//////////////////////////////


	// 하드웨어 백버퍼/깊이버퍼를 사용한다.
	Graphics::m_pDevice->SetRenderTarget( 0, pHWBackBuffer );
	Graphics::m_pDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);

	pHWBackBuffer->Release();
	pHWBackBuffer = NULL;
	pHWDepthStencilBuffer->Release();
	pHWDepthStencilBuffer = NULL;
}

void World::RenderScene(DWORD elapseTime)
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();
	pEffect->SetTexture("Tex_Depth", m_pShadowRenderTarget);	

	if (!m_renderQueueNormal.m_materialOrder.empty())
	{
		Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pNormalVertexDeclaration);
		m_renderQueueNormal.RenderNotAlphaBlendByMaterialOrder(elapseTime,Graphics::m_pInstance->m_vecTechniqueNormal);
	}

	if (!m_renderQueueSkinned.m_materialOrder.empty())
	{
		Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pSkinnedVertexDeclaration);
		m_renderQueueSkinned.RenderNotAlphaBlendByMaterialOrder(elapseTime,Graphics::m_pInstance->m_vecTechniqueSkinned);
	}	

	if (!m_renderQueueNormal.m_sceneOrder.empty())
	{
		m_renderQueueNormal.RenderNotAlphaBlendNormalInstancing(elapseTime,Graphics::m_pInstance->m_vecTechniqueNormalInstancing);		
	}		


	if (!m_renderQueueSkinned.m_sceneOrder.empty())
	{
		m_renderQueueSkinned.RenderNotAlphaBlendSkinnedInstancing(elapseTime,Graphics::m_pInstance->m_vecTechniqueSkinnedInstancing);
	}		


	if (!m_renderQueueNormalAlphaBlend.m_distanceOrder.empty())
	{
		Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pNormalVertexDeclaration);
		m_renderQueueNormalAlphaBlend.RenderAlphaBlendByDistanceOrder(elapseTime,Graphics::m_pInstance->m_vecTechniqueNormal);
	}

	if (!m_renderQueueSkinnedAlphaBlend.m_distanceOrder.empty())
	{
		Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pSkinnedVertexDeclaration);
		m_renderQueueSkinnedAlphaBlend.RenderAlphaBlendByDistanceOrder(elapseTime,Graphics::m_pInstance->m_vecTechniqueSkinned);		
	}
}

void World::SetWorldLightDirection( const D3DXVECTOR3& val )
{
	D3DXVec3Normalize(&m_worldLightDirection,&val);
}



}