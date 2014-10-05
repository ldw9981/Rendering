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
	#define FOV          (PI/4.0f)							// �þ߰�
	#define ASPECT_RATIO (1024/(float)768)		// ȭ���� ��Ⱦ��
	#define NEAR_PLANE   1									// ���� ���
	#define FAR_PLANE    10000								// ���Ÿ� ���

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
	// ����Ÿ���� �����.
	const int shadowMapSize = SHADOWMAP_SIZE;
	if(FAILED(Graphics::m_pDevice->CreateTexture( shadowMapSize, shadowMapSize,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL ) ))
	{
		return false;
	}

	// �׸��� �ʰ� ������ ũ���� ���̹��۵� �������� �Ѵ�.
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
	// ����-�� ����� �����.
	D3DXMATRIX matLightView;
	{
		D3DXVECTOR3 vEyePt( m_WorldLightPosition.x, m_WorldLightPosition.y,  m_WorldLightPosition.z );		
		const D3DXVECTOR3* vLookatPt = m_camera.GetWorldPos();

		vEyePt += *vLookatPt;
		D3DXVECTOR3 vUpVec(    0.0f, 1.0f,  0.0f );
		D3DXMatrixLookAtLH( &matLightView, &vEyePt, vLookatPt, &vUpVec );
	}

	// ����-���� ����� �����.
	D3DXMATRIX matLightProjection;
	{
		//D3DXMatrixPerspectiveFovLH( &matLightProjection, D3DX_PI / 4.0f, 1, 3000, FAR_PLANE );
		D3DXMatrixOrthoLH( &matLightProjection, SHADOWMAP_SIZE,SHADOWMAP_SIZE, NEAR_PLANE, FAR_PLANE );
	}


	// ���� �ϵ���� �����ۿ� ���̹���
	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
	Graphics::m_pDevice->GetRenderTarget(0, &pHWBackBuffer);
	Graphics::m_pDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	//////////////////////////////
	// 1. �׸��� �����
	//////////////////////////////

	// �׸��� ���� ����Ÿ��� ���̹��۸� ����Ѵ�.
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
		Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pNormalVertexDeclation);
		if (!m_renderQueueNormalShadow.IsEmpty())
		{
			m_renderQueueNormalShadow.RenderShadow(Graphics::m_pInstance->m_hTCreateShadowNormal,
				Graphics::m_pInstance->m_hTCreateShadowNormalAlphaTest );
		}		

		Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pSkinnedVertexDeclation);
		if (!m_renderQueueSkinnedShadow.IsEmpty())
		{	
			m_renderQueueSkinnedShadow.RenderShadow(Graphics::m_pInstance->m_hTCreateShadowSkinned,
				Graphics::m_pInstance->m_hTCreateShadowSkinnedAlphaTest );
		}		
	}

	//////////////////////////////
	// 2. �׸��� ������
	//////////////////////////////

	// �ϵ���� �����/���̹��۸� ����Ѵ�.
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

	Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pNormalVertexDeclation);
	if (!m_renderQueueNormal.IsEmpty())
	{
		m_renderQueueNormal.RenderNotAlphaBlend(Graphics::m_pInstance->m_vecTechniqueNormal);
	}

	Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pSkinnedVertexDeclation);
	if (!m_renderQueueSkinned.IsEmpty())
	{
		m_renderQueueSkinned.RenderNotAlphaBlend(Graphics::m_pInstance->m_vecTechniqueSkinned);
	}
	

	Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pNormalVertexDeclation);
	if (!m_renderQueueNormalAlphaBlend.IsEmpty())
	{
		m_renderQueueNormalAlphaBlend.RenderAlphaBlend(Graphics::m_pInstance->m_vecTechniqueNormal,&m_camera);
	}

	Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pSkinnedVertexDeclation);
	if (!m_renderQueueSkinnedAlphaBlend.IsEmpty())
	{
		m_renderQueueSkinnedAlphaBlend.RenderAlphaBlend(Graphics::m_pInstance->m_vecTechniqueSkinned,&m_camera);		
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
	m_renderQueueSkinnedShadow.Clear();

	m_renderQueueNormal.Clear();
	m_renderQueueSkinned.Clear();
	m_renderQueueNormalAlphaBlend.Clear();
	m_renderQueueSkinnedAlphaBlend.Clear();
	
	m_renderQueueTerrain.Clear();




	for ( auto itIn = m_listEntityShadow.begin() ;itIn!=m_listEntityShadow.end() ; ++itIn )
	{
		m_renderQueueNormalShadow.Insert((*itIn)->m_renderQueueNormalShadow);
		m_renderQueueSkinnedShadow.Insert((*itIn)->m_renderQueueSkinnedShadow);
	}


	for ( auto itIn = m_listEntityRender.begin() ;itIn!=m_listEntityRender.end() ; ++itIn )
	{
		m_renderQueueTerrain.Insert((*itIn)->m_renderQueueTerrain);

		m_renderQueueNormal.Insert((*itIn)->m_renderQueueNormal);	
		m_renderQueueSkinned.Insert((*itIn)->m_renderQueueSkinned);		
		m_renderQueueNormalAlphaBlend.Insert((*itIn)->m_renderQueueNormalAlphaBlend);	
		m_renderQueueSkinnedAlphaBlend.Insert((*itIn)->m_renderQueueSkinnedAlphaBlend);					
	}
}

}