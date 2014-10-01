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
	m_WorldLightPosition = D3DXVECTOR4(1500.0f, 500.0f, -1500.0f, 1.0f);
	m_bDebugBound = false;
	m_bEnableShadow = true;
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
	GatherRender();

	Render();


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
	m_listEntityShadow.clear();
	m_listEntityRender.clear();

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
	//Graphics::m_pDevice->SetViewport(&m_ViewPortInfo);
	LPD3DXEFFECT m_pEffect = Graphics::m_pInstance->GetEffect();
	
	UINT passes = 0;
	// 광원-뷰 행렬을 만든다.
	D3DXMATRIX matLightView;
	{
		D3DXVECTOR3 vEyePt( m_WorldLightPosition.x, m_WorldLightPosition.y,  m_WorldLightPosition.z );		
		const D3DXVECTOR3* vLookatPt = m_camera.GetWorldPos();

		vEyePt += *vLookatPt;
		D3DXVECTOR3 vUpVec(    0.0f, 1.0f,  0.0f );
		D3DXMatrixLookAtLH( &matLightView, &vEyePt, vLookatPt, &vUpVec );
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

	if (m_bEnableShadow)
	{
		//1. write depth
		m_pEffect->SetTechnique( Graphics::m_pInstance->m_hTCreateShadowNormal);
		m_pEffect->Begin(&passes, 0);
		m_pEffect->BeginPass(0);
		m_renderQueueNormalShadow.Render();
		m_pEffect->EndPass();
		m_pEffect->End();

		m_pEffect->SetTechnique(Graphics::m_pInstance->m_hTCreateShadowBlend);
		m_pEffect->Begin(&passes, 0);
		m_pEffect->BeginPass(0);
		m_renderQueueSkinnedShadow.Render();
		m_pEffect->EndPass();
		m_pEffect->End();
			
		m_pEffect->SetTechnique( Graphics::m_pInstance->m_hTCreateShadowNormalAlphaTest);
		m_pEffect->Begin(&passes, 0);
		m_pEffect->BeginPass(0);
		m_renderQueueNormalAlphaTestShadow.RenderAlphaTest();
		m_pEffect->EndPass();
		m_pEffect->End();

		m_pEffect->SetTechnique(Graphics::m_pInstance->m_hTCreateShadowBlendAlphaTest);
		m_pEffect->Begin(&passes, 0);
		m_pEffect->BeginPass(0);
		m_renderQueueSkinnedAlphaTestShadow.RenderAlphaTest();
		m_pEffect->EndPass();
		m_pEffect->End();
		
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
	m_pEffect->SetTexture("ShadowMap_Tex", m_pShadowRenderTarget);	

	if (!m_renderQueueTerrain.IsEmpty())
	{
		m_pEffect->SetTechnique(Graphics::m_pInstance->m_hTerrain);
		m_pEffect->Begin(&passes, 0);	
		m_pEffect->BeginPass(0);	
		m_renderQueueTerrain.Render();
		m_pEffect->EndPass();
		m_pEffect->End();
	}

	Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pVertexDeclationNormal);
	int size = Graphics::m_pInstance->m_nTechniqueSize;
	for (int i=0;i<size ;i++)
	{
		if (m_renderQueueNormal[i].IsEmpty())
			continue;

		m_pEffect->SetTechnique(Graphics::m_pInstance->m_pTNormal[i]);
		m_pEffect->Begin(&passes, 0);	
		m_pEffect->BeginPass(0);			
		m_renderQueueNormal[i].Render();
		m_pEffect->EndPass();
		m_pEffect->End();
	}
	for (int i=0;i<size;i++)
	{
		if (m_renderQueueNormalAlphaTest[i].IsEmpty())
			continue;

		m_pEffect->SetTechnique(Graphics::m_pInstance->m_pTNormal[i]);
		m_pEffect->Begin(&passes, 0);	
		m_pEffect->BeginPass(0);			
		m_renderQueueNormalAlphaTest[i].RenderAlphaTest();
		m_pEffect->EndPass();
		m_pEffect->End();
	}

	Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pVertexDeclationBlend);
	for (int i=0;i<size;i++)
	{
		if (m_renderQueueSkinned[i].IsEmpty())
			continue;

		m_pEffect->SetTechnique(Graphics::m_pInstance->m_pTBlend[i]);
		m_pEffect->Begin(&passes, 0);	
		m_pEffect->BeginPass(0);	
		m_renderQueueSkinned[i].Render();		
		m_pEffect->EndPass();
		m_pEffect->End();
	}	
	for (int i=0;i<size;i++)
	{
		if (m_renderQueueSkinnedAlphaTest[i].IsEmpty())
			continue;

		m_pEffect->SetTechnique(Graphics::m_pInstance->m_pTBlend[i]);
		m_pEffect->Begin(&passes, 0);
		m_pEffect->BeginPass(0);	
		m_renderQueueSkinnedAlphaTest[i].RenderAlphaTest();		
		m_pEffect->EndPass();
		m_pEffect->End();
	}

	Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pVertexDeclationNormal);
	for (int i=0;i<size;i++)
	{
		if (m_renderQueueNormalAlphaBlend[i].IsEmpty())
			continue;

		m_pEffect->SetTechnique(Graphics::m_pInstance->m_pTNormal[i]);
		m_pEffect->Begin(&passes, 0);	
		m_pEffect->BeginPass(0);				
		m_renderQueueNormalAlphaBlend[i].RenderAlphaBlendAndTest(&m_camera);
		m_pEffect->EndPass();
		m_pEffect->End();
	}

	Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pVertexDeclationBlend);
	for (int i=0;i<size;i++)
	{
		if (m_renderQueueSkinnedAlphaBlend[i].IsEmpty())
			continue;

		m_pEffect->SetTechnique(Graphics::m_pInstance->m_pTBlend[i]);
		m_pEffect->Begin(&passes, 0);	
		m_pEffect->BeginPass(0);	
		m_renderQueueSkinnedAlphaBlend[i].RenderAlphaBlendAndTest(&m_camera);		
		m_pEffect->EndPass();
		m_pEffect->End();
	}


	if (m_bDebugBound)
	{
		if (!m_listEntityRender.empty())
		{
			m_pEffect->SetTechnique(Graphics::m_pInstance->m_hTLine);
			m_pEffect->Begin(&passes, 0);	
			m_pEffect->BeginPass(0);
			auto itEntityRender = m_listEntityRender.begin();
			for ( ;itEntityRender != m_listEntityRender.end() ; ++itEntityRender )
			{
				(*itEntityRender)->RenderBound();
			}	
			m_pEffect->EndPass();
			m_pEffect->End();	
		}
	}

	if (!m_mapButton.empty())
	{
		m_pEffect->SetTechnique(Graphics::m_pInstance->m_hTGUI);
		m_pEffect->Begin(&passes, 0);	
		m_pEffect->BeginPass(0);
		for ( auto it = m_mapButton.begin();it != m_mapButton.end() ; ++it )
		{
			(*it).second->Render();
		}	
		m_pEffect->EndPass();
		m_pEffect->End();
	}
	
	// SHADOW_MAP
	Graphics::m_pDevice->SetTexture (0, m_pShadowRenderTarget );
	Graphics::m_pDevice->SetFVF(FVF_GUIVERTEX);
	Graphics::m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, & Graphics::m_pInstance->g_vertices[0], sizeof(GUIVERTEX));
	
}

void World::GatherRender()
{
	m_renderQueueNormalShadow.Clear();
	m_renderQueueNormalAlphaTestShadow.Clear();
	m_renderQueueSkinnedShadow.Clear();
	m_renderQueueSkinnedAlphaTestShadow.Clear();
	for (int i=0;i<TECHNIQUE_SIZE;i++)
	{
		m_renderQueueNormal[i].Clear();
		m_renderQueueSkinned[i].Clear();
		m_renderQueueNormalAlphaTest[i].Clear();
		m_renderQueueSkinnedAlphaTest[i].Clear();
		m_renderQueueNormalAlphaBlend[i].Clear();
		m_renderQueueSkinnedAlphaBlend[i].Clear();
	}		
	m_renderQueueTerrain.Clear();




	for ( auto itIn = m_listEntityShadow.begin() ;itIn!=m_listEntityShadow.end() ; ++itIn )
	{
		m_renderQueueNormalShadow.Insert((*itIn)->m_renderQueueNormalShadow);
		m_renderQueueNormalAlphaTestShadow.Insert((*itIn)->m_renderQueueNormalAlphaTestShadow);
		m_renderQueueSkinnedShadow.Insert((*itIn)->m_renderQueueSkinnedShadow);
		m_renderQueueSkinnedAlphaTestShadow.Insert((*itIn)->m_renderQueueSkinnedAlphaTestShadow);
	}


	for ( auto itIn = m_listEntityRender.begin() ;itIn!=m_listEntityRender.end() ; ++itIn )
	{
		m_renderQueueTerrain.Insert((*itIn)->m_renderQueueTerrain);
		for (int i=0;i<TECHNIQUE_SIZE;i++)
		{
			m_renderQueueNormal[i].Insert((*itIn)->m_renderQueueNormal[i]);	
			m_renderQueueSkinned[i].Insert((*itIn)->m_renderQueueSkinned[i]);		
			m_renderQueueNormalAlphaTest[i].Insert((*itIn)->m_renderQueueNormalAlphaTest[i]);	
			m_renderQueueSkinnedAlphaTest[i].Insert((*itIn)->m_renderQueueSkinnedAlphaTest[i]);		
			m_renderQueueNormalAlphaBlend[i].Insert((*itIn)->m_renderQueueNormalAlphaBlend[i]);	
			m_renderQueueSkinnedAlphaBlend[i].Insert((*itIn)->m_renderQueueSkinnedAlphaBlend[i]);		
		}			
	}
}

}