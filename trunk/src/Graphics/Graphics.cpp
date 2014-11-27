#include "StdAfx.h"
#include "Graphics.h"
#include "Foundation/Define.h"
#include "GUI/GUIFont.h"
#include "GUI/GUIButton.h"
#include "RscVertexBuffer.h"
#include "RscTexture.h"
#include "Vertex.h"
#include "MaterialEx.h"
#include "World.h"
#include "Foundation/Trace.h"

#define PI           3.14159265f
#define FOV          (PI/4.0f)							// 시야각
#define ASPECT_RATIO (1024/(float)768)		// 화면의 종횡비
#define NEAR_PLANE   1									// 근접 평면
#define FAR_PLANE    10000								// 원거리 평면

#define T_SIZE 256

#define SHADOWMAP_SIZE 4096
//#define SET_TEXTURE_NULL 


// #define DEBUG_VS
// #define DEBUG_PS


namespace Sophia
{
Graphics*	Graphics::m_pInstance = NULL;
LPDIRECT3DDEVICE9 Graphics::m_pDevice;
Graphics::Graphics(void)
{
	m_pInstance = this;
	m_bDebugBound = false;
	m_nTechniqueSize = (int)pow(2.0f,Material::MAX);
	for (int i=0;i<m_nTechniqueSize;i++)
	{	
		m_vecTechniqueNormal.push_back(NULL);
		m_vecTechniqueSkinned.push_back(NULL);
		m_vecTechniqueNormalInstancing.push_back(NULL);
		m_vecTechniqueSkinnedInstancing.push_back(NULL);
	}
	m_defaultInstancingMax = 128;
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
	HRESULT hr;
	m_width = width;
	m_height = height;	
	m_hWndPresent = hWndPresent;
	char szTemp[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szTemp);
		
	m_pD3D9 = Direct3DCreate9( D3D_SDK_VERSION );
	V( m_pD3D9->GetDeviceCaps( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&m_caps) );      
	
	m_vecRenderTarget.resize(m_caps.NumSimultaneousRTs,(LPDIRECT3DSURFACE9)NULL);

	memset(&m_D3DPP,0,sizeof(m_D3DPP));
	if(bWindowed)
	{
		m_D3DPP.Windowed	 = true;
		m_D3DPP.BackBufferFormat	 = D3DFMT_UNKNOWN;	// 현재의 디스플레이 모드 포맷을 사용
	}
	else
	{
		m_D3DPP.Windowed	 = false;
		m_D3DPP.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
	}
	m_D3DPP.hDeviceWindow = hWndPresent;
	m_D3DPP.BackBufferWidth = m_width;    // set the width of the buffer
	m_D3DPP.BackBufferHeight = m_height;    // set the height of the buffer
	m_D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // dx가 모니터 수직동기화를 하지않음, 동기타임이 될때까지 기다리지않는다.
	m_D3DPP.SwapEffect	 = D3DSWAPEFFECT_DISCARD ;	//백버퍼의 내용이 변경되어도상관없다 
	m_D3DPP.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL; 
	// 플래그를 설정 하면,IDirect3DDevice9::Present 또는 다른 깊이 표면에서의 IDirect3DDevice9::SetDepthStencilSurface 의 호출 후, 스텐실 버퍼의 내용은 무효가 된다. 
	//z 버퍼 데이터의 파기는, 퍼포먼스를 향상시킬 수가 있어 드라이버에 의존한다

	// Zbuffer사용
	m_D3DPP.EnableAutoDepthStencil	= TRUE;		
	m_D3DPP.AutoDepthStencilFormat	= D3DFMT_D16;

	V( m_pD3D9->CreateDevice( 
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		hWndPresent,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,		
		&m_D3DPP, 
		&m_pDevice ));


	if( FAILED( hr ) )
		MessageBox(NULL,"Call to CreateDevice failed!", "ERROR",MB_OK|MB_ICONEXCLAMATION);



	V(m_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE));	

	m_pNewFont = new cGUIFont();	

	m_pDevice->CreateVertexDeclaration(declNormal, &m_pNormalVertexDeclaration);
	m_pDevice->CreateVertexDeclaration(declBlend, &m_pSkinnedVertexDeclaration);
	m_pDevice->CreateVertexDeclaration(declNormalInstance, &m_pNormalInstancingVertexDeclaration);
	m_pDevice->CreateVertexDeclaration(declBlendInstance, &m_pSkinnedInstancingVertexDeclaration);

	
	SetPos(width-T_SIZE,0);

	return true;
}

void Graphics::Finalize()
{
	

	SAFE_RELEASE(m_pNormalVertexDeclaration);
	SAFE_RELEASE(m_pSkinnedVertexDeclaration)
	SAFE_RELEASE(m_pNormalInstancingVertexDeclaration);
	SAFE_RELEASE(m_pSkinnedInstancingVertexDeclaration);
	SAFE_DELETE(m_pNewFont);
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pD3D9);

}


void Graphics::RenderDebugString(int x,int y,const char* szText)
{	
	m_pNewFont->DrawText(x,y,szText);
}

void Graphics::LoadHLSL(const char* szFileName)
{	
	assert(szFileName!=NULL);
	// Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
	// shader debugger. Debugging vertex shaders requires either REF or software vertex 
	// processing, and debugging pixel shaders requires REF.  The 
	// D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
	// shader debugger.  It enables source level debugging, prevents instruction 
	// reordering, prevents dead code elimination, and forces the compiler to compile 
	// against the next higher available software target, which ensures that the 
	// unoptimized shaders do not exceed the shader model limitations.  Setting these 
	// flags will cause slower rendering since the shaders will be unoptimized and 
	// forced into software.  See the DirectX documentation for more information about 
	// using the shader debugger.
	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DXSHADER_DEBUG;
	dwShaderFlags |= D3DXSHADER_SKIPOPTIMIZATION;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif
#ifdef D3DXFX_LARGEADDRESS_HANDLE
	dwShaderFlags |= D3DXFX_LARGEADDRESSAWARE;
#endif

	HRESULT hr;
	LPD3DXBUFFER pErr=NULL;
	if( FAILED( hr = D3DXCreateEffectFromFile(m_pDevice, szFileName, NULL,NULL, dwShaderFlags  , NULL, &m_pEffect, &pErr )))
	{
		MessageBox( NULL, (LPCTSTR)pErr->GetBufferPointer(), "ERROR", MB_OK);
		DXTrace(__FILE__, __LINE__, hr, _T("Error"), TRUE);
		__debugbreak();
		return;
	}

	
	D3DXEFFECT_DESC desc;
	hr = m_pEffect->GetDesc(&desc);

	m_hmWorld = m_pEffect->GetParameterByName( NULL, "gWorldMatrix" );
	m_hmView = m_pEffect->GetParameterByName( NULL, "gViewMatrix" );
	m_hmProjection = m_pEffect->GetParameterByName( NULL, "gProjectionMatrix" );
	m_hmViewProjection = m_pEffect->GetParameterByName( NULL, "gViewProjectionMatrix" );
	m_hmPalette = m_pEffect->GetParameterByName( NULL, "Palette" );
	m_hvWorldLightDirection = m_pEffect->GetParameterByName( NULL, "gWorldLightDirection" );
	m_hvWorldCameraPosition = m_pEffect->GetParameterByName( NULL, "gWorldCameraPosition" );
	m_hmLightView = m_pEffect->GetParameterByName( NULL, "gLightViewMatrix" );
	m_hmLightProjection = m_pEffect->GetParameterByName( NULL, "gLightProjectionMatrix" );
	m_hfMatrixTextureSize = m_pEffect->GetParameterByName( NULL, "gMatrixTextureSize" );

	

	m_hTLine								=m_pEffect->GetTechniqueByName( _T("TLine") );
	m_hTerrain								=m_pEffect->GetTechniqueByName( _T("TTerrain") );
	m_hTSceneNormal 						=m_pEffect->GetTechniqueByName( _T("TSceneNormal") );
	m_hTSceneNormalDiffuse 					=m_pEffect->GetTechniqueByName( _T("TSceneNormalDiffuse") );
	m_hTSceneNormalDiffuseLight				=m_pEffect->GetTechniqueByName( _T("TSceneNormalDiffuseLight") );	
	m_hTSceneNormalDiffuseBump				=m_pEffect->GetTechniqueByName( _T("TSceneNormalDiffuseBump") );
	m_hTSceneNormalDiffuseOpacity			=m_pEffect->GetTechniqueByName( _T("TSceneNormalDiffuseOpacity") );
	m_hTSceneNormalDiffuseSpecular			=m_pEffect->GetTechniqueByName( _T("TSceneNormalDiffuseSpecular") );
	m_hTSceneNormalDiffuseBumpSpecular		=m_pEffect->GetTechniqueByName( _T("TSceneNormalDiffuseBumpSpecular") );

	m_hTSceneNormalInstancingDiffuse		=m_pEffect->GetTechniqueByName( _T("TSceneNormalInstancingDiffuse") );
	m_hTSceneNormalInstancingDiffuseOpacity =m_pEffect->GetTechniqueByName( _T("TSceneNormalInstancingDiffuseOpacity") );
	m_hTSceneNormalInstancingDiffuseLight	=m_pEffect->GetTechniqueByName( _T("TSceneNormalInstancingDiffuseLight") );

	m_hTSceneSkinned						=m_pEffect->GetTechniqueByName( _T("TSceneSkinned") );	
	m_hTSceneSkinnedDiffuse					=m_pEffect->GetTechniqueByName( _T("TSceneSkinnedDiffuse") );	
	m_hTSceneSkinnedInstancingDiffuse		=m_pEffect->GetTechniqueByName( _T("TSceneSkinnedInstancingDiffuse") );

	m_hTShadowNormal						=m_pEffect->GetTechniqueByName( _T("TShadowNormal") );
	m_hTShadowNormalAlphaTest				=m_pEffect->GetTechniqueByName( _T("TShadowNormalAlphaTest") );
	m_hTShadowNormalInstancing				=m_pEffect->GetTechniqueByName( _T("TShadowNormalInstancing") );
	m_hTShadowNormalInstancingAlphaTest		=m_pEffect->GetTechniqueByName( _T("TShadowNormalInstancingAlphaTest") );

	m_hTShadowSkinned						=m_pEffect->GetTechniqueByName( _T("TShadowSkinned") );	
	m_hTShadowSkinnedAlphaTest				=m_pEffect->GetTechniqueByName( _T("TShadowSkinnedAlphaTest") );
	m_hTShadowSkinnedInstancing				=m_pEffect->GetTechniqueByName( _T("TShadowSkinnedInstancing") );	
	m_hTShadowSkinnedInstancingAlphaTest	=m_pEffect->GetTechniqueByName( _T("TShadowSkinnedInstancingAlphaTest") );

	m_hTGUI = m_pEffect->GetTechniqueByName( _T("TGUI") );




	SAFE_RELEASE(pErr);


	std::bitset<Material::MAX> indexRenderQueue;

	m_vecTechniqueNormal[indexRenderQueue.to_ulong()] = m_hTSceneNormal;

	indexRenderQueue.set(Material::DIFFUSE);
	m_vecTechniqueNormal[indexRenderQueue.to_ulong()] = m_hTSceneNormalDiffuse;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::NORMAL);
	m_vecTechniqueNormal[indexRenderQueue.to_ulong()] = m_hTSceneNormalDiffuseBump;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::LIGHT);
	m_vecTechniqueNormal[indexRenderQueue.to_ulong()] = m_hTSceneNormalDiffuseLight;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::OPACITY);
	m_vecTechniqueNormal[indexRenderQueue.to_ulong()] = m_hTSceneNormalDiffuseOpacity;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::SPECULAR);
	m_vecTechniqueNormal[indexRenderQueue.to_ulong()] = m_hTSceneNormalDiffuseSpecular;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::NORMAL);
	indexRenderQueue.set(Material::SPECULAR);
	m_vecTechniqueNormal[indexRenderQueue.to_ulong()] = m_hTSceneNormalDiffuseBumpSpecular;


	for (int i=0;i<m_nTechniqueSize;i++)
	{	
		if (m_vecTechniqueNormal[i] == NULL )	
			m_vecTechniqueNormal[i] = m_hTSceneNormalDiffuse;
	}

	indexRenderQueue = 0;
	m_vecTechniqueSkinned[indexRenderQueue.to_ulong()] = m_hTSceneSkinned;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	m_vecTechniqueSkinned[indexRenderQueue.to_ulong()] = m_hTSceneSkinnedDiffuse;

	for (int i=0;i<m_nTechniqueSize;i++)
	{	
		if (m_vecTechniqueSkinned[i] == NULL )	
			m_vecTechniqueSkinned[i] = m_hTSceneSkinnedDiffuse;
	}


	// m_vecTechniqueNormalInstancing
	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	m_vecTechniqueNormalInstancing[indexRenderQueue.to_ulong()] = m_hTSceneNormalInstancingDiffuse;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::OPACITY);
	m_vecTechniqueNormalInstancing[indexRenderQueue.to_ulong()] = m_hTSceneNormalInstancingDiffuseOpacity;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::LIGHT);
	m_vecTechniqueNormalInstancing[indexRenderQueue.to_ulong()] = m_hTSceneNormalInstancingDiffuseLight;
	

	for (int i=0;i<m_nTechniqueSize;i++)
	{	
		if (m_vecTechniqueNormalInstancing[i] == NULL )	
			m_vecTechniqueNormalInstancing[i] = m_hTSceneNormalInstancingDiffuse;
	}

	// m_vecTechniqueSkinnedInstancing
	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	m_vecTechniqueSkinnedInstancing[indexRenderQueue.to_ulong()] = m_hTSceneSkinnedInstancingDiffuse;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::OPACITY);
	m_vecTechniqueSkinnedInstancing[indexRenderQueue.to_ulong()] = m_hTSceneNormalInstancingDiffuseOpacity;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::LIGHT);
	m_vecTechniqueSkinnedInstancing[indexRenderQueue.to_ulong()] = m_hTSceneNormalInstancingDiffuseLight;


	for (int i=0;i<m_nTechniqueSize;i++)
	{	
		if (m_vecTechniqueSkinnedInstancing[i] == NULL )	
			m_vecTechniqueSkinnedInstancing[i] = m_hTSceneNormalInstancingDiffuse;
	}
	

}


void Graphics::Begin()
{
	m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
	m_pDevice->BeginScene();	
#ifdef SET_TEXTURE_NULL
	for (int i=0; i< 16; i++) 
		m_pDevice->SetTexture(i, NULL);
#endif	
}

void Graphics::End()
{
	m_pDevice->EndScene();
	m_pDevice->Present( NULL, NULL, m_hWndPresent, NULL );	
}

void Graphics::SetEffectVector_WorldLightDirection( D3DXVECTOR4* pVec )
{
	m_pEffect->SetVector(m_hvWorldLightDirection,pVec);
}
void Graphics::SetEffectMatirx_LightView(D3DXMATRIX* pMat )
{
	m_pEffect->SetMatrix(m_hmLightView, pMat);
}

void Graphics::SetEffectMatirx_LightProjection(D3DXMATRIX* pMat )
{
	m_pEffect->SetMatrix(m_hmLightProjection,pMat);	
}

void Graphics::BackupRenderTarget( unsigned int renderTargetIndex )
{
	HRESULT hr;
	V( m_pDevice->GetRenderTarget(renderTargetIndex, &m_vecRenderTarget[renderTargetIndex]) );
}

void Graphics::RestoreRenderTarget( unsigned int renderTargetIndex )
{
	if (m_vecRenderTarget[renderTargetIndex] == NULL)
		return;

	m_pDevice->SetRenderTarget(renderTargetIndex,m_vecRenderTarget[renderTargetIndex]);
	m_vecRenderTarget[renderTargetIndex]->Release();
	m_vecRenderTarget[renderTargetIndex] = NULL;
}

void Graphics::BackupDepthStencilSurface()
{
	HRESULT hr;
	V( Graphics::m_pDevice->GetDepthStencilSurface(&m_depthStencilSurface) );	
}

void Graphics::RestoreDepthStencilSurface()
{
	Graphics::m_pDevice->SetDepthStencilSurface(m_depthStencilSurface);	
	m_depthStencilSurface->Release();
	m_depthStencilSurface = NULL;
}

void Graphics::SetEntityInstancingMax( const char* entityName,int max )
{
	auto ret = m_mapInstancingMax.find(std::string(entityName));
	m_mapInstancingMax[std::string(entityName)] = max;
}

int Graphics::GetEntityInstancingMax( const char* entityName )
{
	auto ret = m_mapInstancingMax.find(std::string(entityName));
	if (ret != m_mapInstancingMax.end())
	{
		return (*ret).second;
	}
	return m_defaultInstancingMax;
}



}