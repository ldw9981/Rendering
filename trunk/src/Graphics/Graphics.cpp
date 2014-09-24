#include "StdAfx.h"
#include "Graphics.h"
#include "Foundation/Define.h"
#include "GUI/GUIFont.h"
#include "GUI/GUIButton.h"
#include "RscVertexBuffer.h"
#include "Vertex.h"
#include "MaterialEx.h"
#include "World.h"

#define PI           3.14159265f
#define FOV          (PI/4.0f)							// 시야각
#define ASPECT_RATIO (1024/(float)768)		// 화면의 종횡비
#define NEAR_PLANE   1									// 근접 평면
#define FAR_PLANE    10000								// 원거리 평면

#define T_SIZE 256

#define SHADOWMAP_SIZE 4096

namespace Sophia
{





Graphics*	Graphics::m_pInstance = NULL;
LPDIRECT3DDEVICE9 Graphics::m_pDevice;
Graphics::Graphics(void)
{
	m_pInstance = this;
	for (int i=0;i<TECHNIQUE_SIZE;i++)
	{	
		m_hTNormal[i] = NULL;
		m_hTBlend[i] = NULL;
	}
	m_viewPortInfo.X = 0;
	m_viewPortInfo.Y = 0;
	m_viewPortInfo.Width = 1024;
	m_viewPortInfo.Height = 768;	
	m_viewPortInfo.MinZ = 0.0f;
	m_viewPortInfo.MaxZ = 1.0f;

	m_pShadowRenderTarget = NULL;
	m_pShadowDepthStencil = NULL;
	m_bDebugBound = false;
}

Graphics::~Graphics(void)
{
}

void Graphics::SetPos( UINT x,UINT y )
{
	// 이미지의 위치, 사이즈로 영역 계산
	m_Rect.left = x;
	m_Rect.top = y;
	m_Rect.right = (LONG)x+T_SIZE;
	m_Rect.bottom = (LONG)y+T_SIZE;	

	// vertex의 수정

	g_vertices[0].vertex = D3DXVECTOR4( (float)m_Rect.left,(float)m_Rect.top,0.0f,0.0f);		
	g_vertices[0].tex.u = 0.0f;
	g_vertices[0].tex.v = 1.0f;

	g_vertices[1].vertex = D3DXVECTOR4( (float)m_Rect.right,(float)m_Rect.top,0.0f,0.0f);		
	g_vertices[1].tex.u = 1.0f;
	g_vertices[1].tex.v = 1.0f;

	g_vertices[2].vertex = D3DXVECTOR4( (float)m_Rect.right,(float)m_Rect.bottom,0.0f,0.0f);		
	g_vertices[2].tex.u = 1.0f;
	g_vertices[2].tex.v = 0.0f;

	g_vertices[3].vertex = D3DXVECTOR4( (float)m_Rect.left,(float)m_Rect.bottom,0.0f,0.0f);		
	g_vertices[3].tex.u = 0.0f;
	g_vertices[3].tex.v = 0.0f;				

}


bool Graphics::Init(HWND hWndPresent,bool bWindowed,int width,int height)
{
	m_viewPortInfo.Width = width;
	m_viewPortInfo.Height = height;	
	m_hWndPresent = hWndPresent;
	char szTemp[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szTemp);

	// 1) D3D를 생성한다.
	m_pD3D9 = Direct3DCreate9( D3D_SDK_VERSION );


	// 2) Device를 생성을 위한 Parameter를 설정한다.
	memset(&m_D3DPP,0,sizeof(m_D3DPP));
	if(bWindowed)
	{
		m_D3DPP.Windowed	 = true;
		m_D3DPP.BackBufferFormat	 = D3DFMT_UNKNOWN;
	}
	else
	{
		m_D3DPP.Windowed	 = false;
		m_D3DPP.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
		m_D3DPP.BackBufferWidth = m_viewPortInfo.Width;    // set the width of the buffer
		m_D3DPP.BackBufferHeight = m_viewPortInfo.Height;    // set the height of the buffer
	}
	m_D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; 
	m_D3DPP.SwapEffect	 = D3DSWAPEFFECT_DISCARD;


	// Zbuffer사용
	m_D3DPP.EnableAutoDepthStencil	= TRUE;		
	m_D3DPP.AutoDepthStencilFormat	= D3DFMT_D16;


	// 3) Device를 생성한다.

	HRESULT hr= m_pD3D9->CreateDevice( 
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		hWndPresent,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,		
		&m_D3DPP, 
		&m_pDevice );


	if( FAILED( hr ) )
		MessageBox(NULL,"Call to CreateDevice failed!", "ERROR",MB_OK|MB_ICONEXCLAMATION);

	m_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);


	m_pNewFont = new cGUIFont();	

	m_pDevice->CreateVertexDeclaration(declNormal, &m_pVertexDeclationNormal);
	m_pDevice->CreateVertexDeclaration(declBlend, &m_pVertexDeclationBlend);


	// 렌더타깃을 만든다.
	const int shadowMapSize = SHADOWMAP_SIZE;
	if(FAILED(m_pDevice->CreateTexture( shadowMapSize, shadowMapSize,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL ) ))
	{
		return false;
	}

	// 그림자 맵과 동일한 크기의 깊이버퍼도 만들어줘야 한다.
	if(FAILED(m_pDevice->CreateDepthStencilSurface(shadowMapSize, shadowMapSize,
		D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE,
		&m_pShadowDepthStencil, NULL)))
	{
		return false;
	}
	SetPos(1024-T_SIZE,0);
	return true;
}

void Graphics::Finalize()
{
	SAFE_RELEASE(m_pShadowDepthStencil);
	SAFE_RELEASE(m_pShadowRenderTarget);
	SAFE_RELEASE(m_pVertexDeclationNormal);
	SAFE_RELEASE(m_pVertexDeclationBlend);
	SAFE_DELETE(m_pNewFont);	
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pD3D9);
}


void Graphics::RenderDebugString(int x,int y,const char* szText)
{	
	m_pNewFont->DrawText(x,y,szText);
}

void Graphics::LoadHLSL(const char* szFileName)
{
	if (!szFileName)
	{
		return;
	}

	HRESULT hr;
	LPD3DXBUFFER pErr=NULL;
	if( FAILED( hr = D3DXCreateEffectFromFile(m_pDevice, szFileName, NULL, NULL,D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION , NULL, &m_pEffect, &pErr )))
	{
		MessageBox( NULL, (LPCTSTR)pErr->GetBufferPointer(), "ERROR", MB_OK);
		DXTrace(__FILE__, __LINE__, hr, _T("Error"), TRUE);
		return;
	}

	D3DXEFFECT_DESC desc;
	hr = m_pEffect->GetDesc(&desc);

	m_hTLine = m_pEffect->GetTechniqueByName( _T("TLine") );
	m_hTerrain = m_pEffect->GetTechniqueByName( _T("TTerrain") );
	m_hTPhong = m_pEffect->GetTechniqueByName( _T("TPhong") );
	m_hTPhongDiffuse = m_pEffect->GetTechniqueByName( _T("TPhongDiffuse") );
	m_hTPhongDiffuseLight = m_pEffect->GetTechniqueByName( _T("TPhongDiffuseLight") );	
	m_hTPhongDiffuseBump = m_pEffect->GetTechniqueByName( _T("TPhongDiffuseBump") );
	m_hTPhongDiffuseOpacity = m_pEffect->GetTechniqueByName( _T("TPhongDiffuseOpacity") );

	m_hTSkinningPhong = m_pEffect->GetTechniqueByName( _T("TSkinningPhong") );	
	m_hTSkinningPhongDiffuse = m_pEffect->GetTechniqueByName( _T("TSkinningPhongDiffuse") );	
	m_hTSkinningPhongDiffuse = m_pEffect->GetTechniqueByName( _T("TSkinningPhongDiffuse") );
	m_hTCreateShadowNormal = m_pEffect->GetTechniqueByName( _T("CreateShadowShader") );
	m_hTCreateShadowBlend = m_pEffect->GetTechniqueByName( _T("TShadowSkinning") );

	m_hTCreateShadowNormalAlphaTest = m_pEffect->GetTechniqueByName( _T("CreateShadowAlphaTestShader") );
	m_hTCreateShadowBlendAlphaTest = m_pEffect->GetTechniqueByName( _T("TShadowSkinningAlphaTest") );

	m_hTGUI = m_pEffect->GetTechniqueByName( _T("TGUI") );

	m_hmWorld = m_pEffect->GetParameterByName( NULL, "gWorldMatrix" );
	m_hmView = m_pEffect->GetParameterByName( NULL, "gViewMatrix" );
	m_hmProjection = m_pEffect->GetParameterByName( NULL, "gProjectionMatrix" );
	m_hmViewProjection = m_pEffect->GetParameterByName( NULL, "gViewProjectionMatrix" );
	m_hmPalette = m_pEffect->GetParameterByName( NULL, "Palette" );


	m_hvWorldLightPosition = m_pEffect->GetParameterByName( NULL, "gWorldLightPosition" );
	m_hvWorldCameraPosition = m_pEffect->GetParameterByName( NULL, "gWorldCameraPosition" );

	m_hmLightView = m_pEffect->GetParameterByName( NULL, "gLightViewMatrix" );
	m_hmLightProjection = m_pEffect->GetParameterByName( NULL, "gLightProjectionMatrix" );


	SAFE_RELEASE(pErr);


	std::bitset<Material::MAX> indexRenderQueue;

	m_hTNormal[indexRenderQueue.to_ulong()] = m_hTPhong;

	indexRenderQueue.set(Material::DIFFUSE);
	m_hTNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuse;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::NORMAL);
	m_hTNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseBump;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::LIGHT);
	m_hTNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseLight;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::OPACITY);
	m_hTNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseOpacity;

	for (int i=0;i<TECHNIQUE_SIZE;i++)
	{	
		if (m_hTNormal[i] == NULL )	
			m_hTNormal[i] = m_hTPhongDiffuse;
	}

	indexRenderQueue.reset();
	m_hTBlend[indexRenderQueue.to_ulong()] = m_hTSkinningPhong;
	for (int i=0;i<TECHNIQUE_SIZE;i++)
	{	
		if (m_hTBlend[i] == NULL )	
			m_hTBlend[i] = m_hTSkinningPhongDiffuse;
	}


}


void Graphics::Begin()
{
	m_pDevice->SetViewport(&m_viewPortInfo);
	m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
	m_pDevice->BeginScene();	
}

void Graphics::End()
{
	m_pDevice->EndScene();
	m_pDevice->Present( NULL, NULL, m_hWndPresent, NULL );	
}

void Graphics::RenderEX( World* pWorld )
{
	cCameraNode* pCamera = &pWorld->m_camera;

	UINT passes = 0;
	m_pDevice->SetViewport(&pWorld->m_ViewPortInfo);

	// 광원-뷰 행렬을 만든다.
	D3DXMATRIX matLightView;
	{
		D3DXVECTOR3 vEyePt( pWorld->m_WorldLightPosition.x, pWorld->m_WorldLightPosition.y,  pWorld->m_WorldLightPosition.z );		
		D3DXVECTOR3 vLookatPt;
		pCamera->GetWorldPos(vLookatPt);

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
	m_pDevice->GetRenderTarget(0, &pHWBackBuffer);
	m_pDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	//////////////////////////////
	// 1. 그림자 만들기
	//////////////////////////////

	// 그림자 맵의 렌더타깃과 깊이버퍼를 사용한다.
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if( SUCCEEDED( m_pShadowRenderTarget->GetSurfaceLevel( 0, &pShadowSurface ) ) )
	{
		m_pDevice->SetRenderTarget( 0, pShadowSurface );
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	m_pDevice->SetDepthStencilSurface( m_pShadowDepthStencil );
	m_pDevice->Clear( 0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0 );

	m_pEffect->SetVector(m_hvWorldLightPosition,&pWorld->m_WorldLightPosition);
	m_pEffect->SetMatrix(m_hmLightView, &matLightView);
	m_pEffect->SetMatrix(m_hmLightProjection, &matLightProjection);	


	//1. write depth
	m_pEffect->SetTechnique(m_hTCreateShadowNormal);
	m_pEffect->Begin(&passes, 0);
	m_pEffect->BeginPass(0);
	pWorld->m_renderQueueNormalShadow.Render();
	m_pEffect->EndPass();
	m_pEffect->End();

	m_pEffect->SetTechnique(m_hTCreateShadowBlend);
	m_pEffect->Begin(&passes, 0);
	m_pEffect->BeginPass(0);
	pWorld->m_renderQueueBlendShadow.Render();
	m_pEffect->EndPass();
	m_pEffect->End();



	//////////////////////////////
	// 2. 그림자 입히기
	//////////////////////////////

	// 하드웨어 백버퍼/깊이버퍼를 사용한다.
	m_pDevice->SetRenderTarget( 0, pHWBackBuffer );
	m_pDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);

	pHWBackBuffer->Release();
	pHWBackBuffer = NULL;
	pHWDepthStencilBuffer->Release();
	pHWDepthStencilBuffer = NULL;

	m_pEffect->SetTexture("ShadowMap_Tex", m_pShadowRenderTarget);

	for (int i=0;i<TECHNIQUE_SIZE;i++)
	{
		m_pEffect->SetTechnique(m_hTNormal[i]);
		m_pEffect->Begin(&passes, 0);	// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다
		m_pEffect->BeginPass(0);	
		pWorld->m_renderQueueNormal[i].Render();		
		m_pEffect->EndPass();
		m_pEffect->End();
	}

	for (int i=0;i<TECHNIQUE_SIZE;i++)
	{
		m_pEffect->SetTechnique(m_hTBlend[i]);
		m_pEffect->Begin(&passes, 0);	// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다
		m_pEffect->BeginPass(0);	
		pWorld->m_renderQueueBlend[i].Render();
		m_pEffect->EndPass();
		m_pEffect->End();
	}

	m_pEffect->SetTechnique(m_hTerrain);
	m_pEffect->Begin(&passes, 0);	// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다
	m_pEffect->BeginPass(0);	
	pWorld->m_renderQueueTerrain.Render();
	m_pEffect->EndPass();
	m_pEffect->End();

	
	if (m_bDebugBound)
	{
		m_pEffect->SetTechnique(m_hTLine);
		m_pEffect->Begin(&passes, 0);	// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다
		m_pEffect->BeginPass(0);
		for ( auto itEntityRender = pWorld->m_listEntityRender.begin() ;itEntityRender != pWorld->m_listEntityRender.end() ; ++itEntityRender )
		{
			(*itEntityRender)->RenderBound();
		}	
		m_pEffect->EndPass();
		m_pEffect->End();	
	}
	
	m_pDevice->SetTexture (0, m_pShadowRenderTarget );
	m_pDevice->SetFVF(FVF_GUIVERTEX);
	m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, &g_vertices[0], sizeof(GUIVERTEX));
}

void Graphics::Render( World* pWorld )
{
	m_pDevice->SetViewport(&pWorld->m_ViewPortInfo);
	cCameraNode* pCamera = &pWorld->m_camera;
	UINT passes = 0;
	// 광원-뷰 행렬을 만든다.
	D3DXMATRIX matLightView;
	{
		D3DXVECTOR3 vEyePt( pWorld->m_WorldLightPosition.x, pWorld->m_WorldLightPosition.y,  pWorld->m_WorldLightPosition.z );		
		D3DXVECTOR3 vLookatPt;
		pCamera->GetWorldPos(vLookatPt);

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
	m_pDevice->GetRenderTarget(0, &pHWBackBuffer);
	m_pDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	//////////////////////////////
	// 1. 그림자 만들기
	//////////////////////////////

	// 그림자 맵의 렌더타깃과 깊이버퍼를 사용한다.
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if( SUCCEEDED( m_pShadowRenderTarget->GetSurfaceLevel( 0, &pShadowSurface ) ) )
	{
		m_pDevice->SetRenderTarget( 0, pShadowSurface );
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	m_pDevice->SetDepthStencilSurface( m_pShadowDepthStencil );
	m_pDevice->Clear( 0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0 );

	m_pEffect->SetVector(m_hvWorldLightPosition,&pWorld->m_WorldLightPosition);
	m_pEffect->SetMatrix(m_hmLightView, &matLightView);
	m_pEffect->SetMatrix(m_hmLightProjection, &matLightProjection);	


	//1. write depth
	m_pEffect->SetTechnique(m_hTCreateShadowNormal);
	m_pEffect->Begin(&passes, 0);
	m_pEffect->BeginPass(0);
	auto itEntityShadow = pWorld->m_listEntityShadow.begin();
	for ( ;itEntityShadow != pWorld->m_listEntityShadow.end() ; ++itEntityShadow )
	{
		(*itEntityShadow)->m_renderQueueNormalShadow.Render();
	}		
	m_pEffect->EndPass();
	m_pEffect->End();

	m_pEffect->SetTechnique(m_hTCreateShadowBlend);
	m_pEffect->Begin(&passes, 0);
	m_pEffect->BeginPass(0);
	itEntityShadow = pWorld->m_listEntityShadow.begin();
	for ( ;itEntityShadow != pWorld->m_listEntityShadow.end() ; ++itEntityShadow )
	{
		(*itEntityShadow)->m_renderQueueBlendShadow.Render();
	}	
	m_pEffect->EndPass();
	m_pEffect->End();




	//////////////////////////////
	// 2. 그림자 입히기
	//////////////////////////////

	// 하드웨어 백버퍼/깊이버퍼를 사용한다.
	m_pDevice->SetRenderTarget( 0, pHWBackBuffer );
	m_pDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);

	pHWBackBuffer->Release();
	pHWBackBuffer = NULL;
	pHWDepthStencilBuffer->Release();
	pHWDepthStencilBuffer = NULL;

	m_pEffect->SetTexture("ShadowMap_Tex", m_pShadowRenderTarget);

	for (int i=0;i<TECHNIQUE_SIZE;i++)
	{
		m_pEffect->SetTechnique(m_hTNormal[i]);
		m_pEffect->Begin(&passes, 0);	// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다
		m_pEffect->BeginPass(0);	

		auto itEntityRender = pWorld->m_listEntityRender.begin();
		for ( ;itEntityRender != pWorld->m_listEntityRender.end() ; ++itEntityRender )
		{
			(*itEntityRender)->m_renderQueueNormal[i].Render();
		}	

		m_pEffect->EndPass();
		m_pEffect->End();
	}

	for (int i=0;i<TECHNIQUE_SIZE;i++)
	{
		m_pEffect->SetTechnique(m_hTBlend[i]);
		m_pEffect->Begin(&passes, 0);	// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다
		m_pEffect->BeginPass(0);	

		auto itEntityRender = pWorld->m_listEntityRender.begin();
		for ( ;itEntityRender != pWorld->m_listEntityRender.end() ; ++itEntityRender )
		{
			(*itEntityRender)->m_renderQueueBlend[i].Render();
		}	

		m_pEffect->EndPass();
		m_pEffect->End();
	}

	m_pEffect->SetTechnique(m_hTerrain);
	m_pEffect->Begin(&passes, 0);	// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다
	m_pEffect->BeginPass(0);	

	auto itEntityRender = pWorld->m_listEntityRender.begin();
	for ( ;itEntityRender != pWorld->m_listEntityRender.end() ; ++itEntityRender )
	{
		(*itEntityRender)->m_renderQueueTerrain.Render();
	}	

	m_pEffect->EndPass();
	m_pEffect->End();

	if (m_bDebugBound)
	{
		m_pEffect->SetTechnique(m_hTLine);
		m_pEffect->Begin(&passes, 0);	// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다
		m_pEffect->BeginPass(0);
		itEntityRender = pWorld->m_listEntityRender.begin();
		for ( ;itEntityRender != pWorld->m_listEntityRender.end() ; ++itEntityRender )
		{
			(*itEntityRender)->RenderBound();
		}	
		m_pEffect->EndPass();
		m_pEffect->End();	
	}

	m_pEffect->SetTechnique(m_hTGUI);
	m_pEffect->Begin(&passes, 0);	// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다
	m_pEffect->BeginPass(0);
	for ( auto it = pWorld->m_mapButton.begin();it != pWorld->m_mapButton.end() ; ++it )
	{
		(*it).second->Render();
	}	
	m_pEffect->EndPass();
	m_pEffect->End();

	// SHADOW_MAP
	m_pDevice->SetTexture (0, m_pShadowRenderTarget );
	m_pDevice->SetFVF(FVF_GUIVERTEX);
	m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, &g_vertices[0], sizeof(GUIVERTEX));
}

void Graphics::SetEffectVector_WorldLightPosition(D3DXVECTOR4* pVec )
{
	m_pEffect->SetVector(m_hvWorldLightPosition,pVec);
}

void Graphics::SetEffectMatirx_LightView(D3DXMATRIX* pMat )
{
	m_pEffect->SetMatrix(m_hmLightView, pMat);
}

void Graphics::SetEffectMatirx_LightProjection(D3DXMATRIX* pMat )
{
	m_pEffect->SetMatrix(m_hmLightProjection,pMat);	
}

}