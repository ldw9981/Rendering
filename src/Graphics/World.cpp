#include "StdAfx.h"
#include "World.h"
#include "GUI/GUIButton.h"
#include "GUI/GUIFont.h"
#include "Graphics.h"
#include "Scene/CameraNode.h"
#include "Scene/ZTerrain.h"
#include "Foundation/Define.h"
namespace Sophia
{
	#define PI           3.14159265f
	#define FOV          (PI/4.0f)							// 시야각
	#define ASPECT_RATIO (1024/(float)768)		// 화면의 종횡비
	#define NEAR_PLANE   1									// 근접 평면
	#define FAR_PLANE    10000								// 원거리 평면

	#define T_SIZE 256

	#define SHADOWMAP_SIZE 4096

World::World(void)
{
	m_ViewPortInfo.MinZ = 0.0f;
	m_ViewPortInfo.MaxZ = 1.0f;
	m_ViewPortInfo.X = 0;
	m_ViewPortInfo.Y = 0;
	m_ViewPortInfo.Width = 0;
	m_ViewPortInfo.Height = 0;
	m_WorldLightPosition = D3DXVECTOR4(500.0f, 5500.0f, -500.0f, 1.0f);
	m_bDebugBound = false;
}


World::~World(void)
{
}

Entity* World::CreateEntity()
{
	Entity* pEntity = new Entity;	
	m_listEntity.push_back(pEntity);
	pEntity->m_itEntityList = --m_listEntity.end();	
	return pEntity;
}

void World::ProcessRender()
{
	m_camera.Render();
	// scene 
	CullFrustum();

	Render();

	m_listEntityShadow.clear();
	m_listEntityRender.clear();
}

void World::Update( DWORD elapseTime )
{
	m_camera.Update(elapseTime);
	for ( auto itIn = m_listEntity.begin() ;itIn!=m_listEntity.end() ; ++itIn )
	{
		(*itIn)->Update(elapseTime);
	}

	for ( auto itIn = m_mapButton.begin() ;itIn!=m_mapButton.end() ; ++itIn )
	{
		(*itIn).second->Update(elapseTime);
	}
}

void World::CullFrustum()
{
	Frustum& frustum = m_camera.GetFrustum();

	for ( auto itIn = m_listEntity.begin() ;itIn!=m_listEntity.end() ; ++itIn )
	{
		if( (*itIn)->Cull(&frustum,500.0f) == false )
			continue;	


		m_listEntityShadow.push_back(*itIn);

		if( (*itIn)->Cull(&frustum,0.0f) == false )
			continue;	

		m_listEntityRender.push_back(*itIn);
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
	m_listEntity.erase(pEntity->m_itEntityList);
	delete pEntity;
}

ZTerrain* World::CreateTerrain( D3DXVECTOR3* pvfScale, const char* lpBMPFilename, const char* lpTexFilename )
{
	ZTerrain* pEntity = new ZTerrain;	
	pEntity->Create(pvfScale,lpBMPFilename,lpTexFilename);
	m_listEntity.push_back(pEntity);
	pEntity->m_itEntityList = --m_listEntity.end();	
	return pEntity;
}

void World::DeleteTerrain( ZTerrain* pEntity )
{
	m_listEntity.erase(pEntity->m_itEntityList);
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
	// 렌더타깃을 만든다.
	const int shadowMapSize = SHADOWMAP_SIZE;
	if(FAILED(Graphics::m_pDevice->CreateTexture( shadowMapSize, shadowMapSize,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL ) ))
	{
		return false;
	}

	// 그림자 맵과 동일한 크기의 깊이버퍼도 만들어줘야 한다.
	if(FAILED(Graphics::m_pDevice->CreateDepthStencilSurface(shadowMapSize, shadowMapSize,
		D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE,
		&m_pShadowDepthStencil, NULL)))
	{
		return false;
	}
	return true;
}

void World::Finalize()
{
	SAFE_RELEASE(m_pShadowDepthStencil);
	SAFE_RELEASE(m_pShadowRenderTarget);
}

void World::Render()
{
	Graphics::m_pDevice->SetViewport(&m_ViewPortInfo);
	LPD3DXEFFECT m_pEffect = Graphics::m_pInstance->GetEffect();
	
	UINT passes = 0;
	// 광원-뷰 행렬을 만든다.
	D3DXMATRIX matLightView;
	{
		D3DXVECTOR3 vEyePt( m_WorldLightPosition.x, m_WorldLightPosition.y,  m_WorldLightPosition.z );		
		D3DXVECTOR3 vLookatPt;
		m_camera.GetWorldPos(vLookatPt);

		vEyePt += vLookatPt;
		D3DXVECTOR3 vUpVec(    0.0f, 1.0f,  0.0f );
		D3DXMatrixLookAtLH( &matLightView, &vEyePt, &vLookatPt, &vUpVec );
	}

	// 광원-투영 행렬을 만든다.
	D3DXMATRIX matLightProjection;
	{
		//D3DXMatrixPerspectiveFovLH( &matLightProjection, D3DX_PI / 4.0f, 1, 3000, FAR_PLANE );
		D3DXMatrixOrthoLH( &matLightProjection, SHADOWMAP_SIZE,SHADOWMAP_SIZE, NEAR_PLANE, FAR_PLANE );
	}


	// 현재 하드웨어 벡버퍼와 깊이버퍼
	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
	Graphics::m_pDevice->GetRenderTarget(0, &pHWBackBuffer);
	Graphics::m_pDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	//////////////////////////////
	// 1. 그림자 만들기
	//////////////////////////////

	// 그림자 맵의 렌더타깃과 깊이버퍼를 사용한다.
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if( SUCCEEDED( m_pShadowRenderTarget->GetSurfaceLevel( 0, &pShadowSurface ) ) )
	{
		Graphics::m_pDevice->SetRenderTarget( 0, pShadowSurface );
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	Graphics::m_pDevice->SetDepthStencilSurface( m_pShadowDepthStencil );
	Graphics::m_pDevice->Clear( 0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0 );

	Graphics::m_pInstance->SetEffectVector_WorldLightPosition(&m_WorldLightPosition);
	Graphics::m_pInstance->SetEffectMatirx_LightView(&matLightView);
	Graphics::m_pInstance->SetEffectMatirx_LightProjection(&matLightProjection);

	
	//1. write depth
	m_pEffect->SetTechnique( Graphics::m_pInstance->m_hTCreateShadowNormal);
	m_pEffect->Begin(&passes, 0);
	m_pEffect->BeginPass(0);
	auto itEntityShadow = m_listEntityShadow.begin();
	for ( ;itEntityShadow != m_listEntityShadow.end() ; ++itEntityShadow )
	{
		(*itEntityShadow)->m_renderQueueNormalShadow.Render();
	}		
	m_pEffect->EndPass();
	m_pEffect->End();

	m_pEffect->SetTechnique(Graphics::m_pInstance->m_hTCreateShadowBlend);
	m_pEffect->Begin(&passes, 0);
	m_pEffect->BeginPass(0);
	itEntityShadow = m_listEntityShadow.begin();
	for ( ;itEntityShadow != m_listEntityShadow.end() ; ++itEntityShadow )
	{
		(*itEntityShadow)->m_renderQueueBlendShadow.Render();
	}	
	m_pEffect->EndPass();
	m_pEffect->End();
	

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

	m_pEffect->SetTexture("ShadowMap_Tex", m_pShadowRenderTarget);

	for (int i=0;i<TECHNIQUE_SIZE;i++)
	{
		m_pEffect->SetTechnique(Graphics::m_pInstance->m_hTNormal[i]);
		m_pEffect->Begin(&passes, 0);	// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다
		m_pEffect->BeginPass(0);	

		
		auto itEntityRender = m_listEntityRender.begin();
		for ( ;itEntityRender != m_listEntityRender.end() ; ++itEntityRender )
		{
			(*itEntityRender)->m_renderQueueNormal[i].Render();
		}	

		m_pEffect->EndPass();
		m_pEffect->End();
	}

	for (int i=0;i<TECHNIQUE_SIZE;i++)
	{
		m_pEffect->SetTechnique(Graphics::m_pInstance->m_hTBlend[i]);
		m_pEffect->Begin(&passes, 0);	// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다
		m_pEffect->BeginPass(0);	

		auto itEntityRender = m_listEntityRender.begin();
		for ( ;itEntityRender != m_listEntityRender.end() ; ++itEntityRender )
		{
			(*itEntityRender)->m_renderQueueBlend[i].Render();
		}	

		m_pEffect->EndPass();
		m_pEffect->End();
	}

	m_pEffect->SetTechnique(Graphics::m_pInstance->m_hTerrain);
	m_pEffect->Begin(&passes, 0);	// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다
	m_pEffect->BeginPass(0);	

	auto itEntityRender = m_listEntityRender.begin();
	for ( ;itEntityRender != m_listEntityRender.end() ; ++itEntityRender )
	{
		(*itEntityRender)->m_renderQueueTerrain.Render();
	}	

	m_pEffect->EndPass();
	m_pEffect->End();

	if (m_bDebugBound)
	{
		m_pEffect->SetTechnique(Graphics::m_pInstance->m_hTLine);
		m_pEffect->Begin(&passes, 0);	// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다
		m_pEffect->BeginPass(0);
		itEntityRender = m_listEntityRender.begin();
		for ( ;itEntityRender != m_listEntityRender.end() ; ++itEntityRender )
		{
			(*itEntityRender)->RenderBound();
		}	
		m_pEffect->EndPass();
		m_pEffect->End();	
	}

	m_pEffect->SetTechnique(Graphics::m_pInstance->m_hTGUI);
	m_pEffect->Begin(&passes, 0);	// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다
	m_pEffect->BeginPass(0);
	for ( auto it = m_mapButton.begin();it != m_mapButton.end() ; ++it )
	{
		(*it).second->Render();
	}	
	m_pEffect->EndPass();
	m_pEffect->End();

	/*
	// SHADOW_MAP
	Graphics::m_pDevice->SetTexture (0, m_pShadowRenderTarget );
	Graphics::m_pDevice->SetFVF(FVF_GUIVERTEX);
	Graphics::m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, &g_vertices[0], sizeof(GUIVERTEX));
	*/
}

}