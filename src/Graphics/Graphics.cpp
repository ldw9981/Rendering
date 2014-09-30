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
	m_viewPortInfo.X = 0;
	m_viewPortInfo.Y = 0;
	m_viewPortInfo.Width = 1024;
	m_viewPortInfo.Height = 768;	
	m_viewPortInfo.MinZ = 0.0f;
	m_viewPortInfo.MaxZ = 1.0f;

	m_pShadowRenderTarget = NULL;
	m_pShadowDepthStencil = NULL;
	m_bDebugBound = false;

	m_nTechniqueSize = (int)pow(2.0f,Material::MAX);
	m_pTNormal = new D3DXHANDLE[m_nTechniqueSize];
	m_pTBlend = new D3DXHANDLE[m_nTechniqueSize];
	for (int i=0;i<m_nTechniqueSize;i++)
	{	
		m_pTNormal[i] = NULL;
		m_pTBlend[i] = NULL;
	}
}

Graphics::~Graphics(void)
{
	delete[] m_pTNormal;
	delete[] m_pTBlend;
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
	m_hTPhongDiffuseSpecular = m_pEffect->GetTechniqueByName( _T("TPhongDiffuseSpecular") );
	m_hTPhongDiffuseBumpSpecular = m_pEffect->GetTechniqueByName( _T("TPhongDiffuseBumpSpecular") );

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

	m_pTNormal[indexRenderQueue.to_ulong()] = m_hTPhong;

	indexRenderQueue.set(Material::DIFFUSE);
	m_pTNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuse;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::NORMAL);
	m_pTNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseBump;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::LIGHT);
	m_pTNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseLight;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::OPACITY);
	m_pTNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseOpacity;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::SPECULAR);
	m_pTNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseSpecular;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::NORMAL);
	indexRenderQueue.set(Material::SPECULAR);
	m_pTNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseBumpSpecular;


	for (int i=0;i<m_nTechniqueSize;i++)
	{	
		if (m_pTNormal[i] == NULL )	
			m_pTNormal[i] = m_hTPhongDiffuse;
	}

	indexRenderQueue = 0;
	m_pTBlend[indexRenderQueue.to_ulong()] = m_hTSkinningPhong;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	m_pTBlend[indexRenderQueue.to_ulong()] = m_hTSkinningPhongDiffuse;

	for (int i=0;i<m_nTechniqueSize;i++)
	{	
		if (m_pTBlend[i] == NULL )	
			m_pTBlend[i] = m_hTSkinningPhongDiffuse;
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