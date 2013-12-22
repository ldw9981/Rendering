#include "StdAfx.h"
#include "Server.h"
#include "Foundation/Define.h"
#include "GUI/GUIFont.h"
#include "RscVertexBuffer.h"
#include "Vertex.h"
#include "MaterialEx.h"

namespace D3D9
{

Server*	Server::g_pServer = NULL;

Server::Server(void)
{
	g_pServer = this;
	for (int i=0;i<16;i++)
	{	
		m_listRenderQueue[i].m_hTechnique = NULL;
		m_listRenderQueueSkinned[i].m_hTechnique = NULL;
	}
}

Server::~Server(void)
{
}

bool Server::Init()
{
	char szTemp[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szTemp);

	// 1) D3D를 생성한다.
	m_pD3D9 = Direct3DCreate9( D3D_SDK_VERSION );


	// 2) Device를 생성을 위한 Parameter를 설정한다.
	memset(&m_D3DPP,0,sizeof(m_D3DPP));
#ifdef _DEBUG

	m_D3DPP.Windowed	 = true;
	m_D3DPP.BackBufferFormat	 = D3DFMT_UNKNOWN;
#else 
	m_D3DPP.Windowed	 = false;
	m_D3DPP.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
	m_D3DPP.BackBufferWidth = 1024;    // set the width of the buffer
	m_D3DPP.BackBufferHeight = 768;    // set the height of the buffer
#endif

	m_D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; 
	m_D3DPP.SwapEffect	 = D3DSWAPEFFECT_DISCARD;
	

	// Zbuffer사용
	m_D3DPP.EnableAutoDepthStencil	= TRUE;		
	m_D3DPP.AutoDepthStencilFormat	= D3DFMT_D16;


	// 3) Device를 생성한다.

	HRESULT hr= m_pD3D9->CreateDevice( 
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,		
		&m_D3DPP, 
		&m_pD3DDevice );


	if( FAILED( hr ) )
		MessageBox(NULL,"Call to CreateDevice failed!", "ERROR",MB_OK|MB_ICONEXCLAMATION);

	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);

	m_pD3DDevice->GetViewport(&m_viewPortInfo);

	D3DMATERIAL9 mtrl;	
	ZeroMemory(&mtrl,sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = 1.0f;
	mtrl.Diffuse.g = 1.0f;
	mtrl.Diffuse.b = 1.0f;
	mtrl.Diffuse.a = 1.0f;

	mtrl.Ambient.r = 1.0f;
	mtrl.Ambient.g = 1.0f;
	mtrl.Ambient.b = 1.0f;
	mtrl.Ambient.a = 1.0f;

	m_pD3DDevice->SetMaterial(&mtrl);


	D3DLIGHT9 light;
	ZeroMemory(&light,sizeof(D3DLIGHT9));
	light.Type	=	D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	light.Direction = D3DXVECTOR3(0.0f,-1.0f,1.0f);		// 타겟 방향
	//	light.Range = 1000.0f;

	m_pD3DDevice->SetLight(0,&light);
	m_pD3DDevice->LightEnable(0,TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
	m_pNewFont = new cGUIFont();	

	m_pD3DDevice->CreateVertexDeclaration(declNormal, &m_pVertexDeclationNormal);
	m_pD3DDevice->CreateVertexDeclaration(declBlend, &m_pVertexDeclationBlend);
	return true;
}

void Server::Uninit()
{
	SAFE_RELEASE(m_pVertexDeclationNormal);
	SAFE_RELEASE(m_pVertexDeclationBlend);
	SAFE_DELETE(m_pNewFont);	
	SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_pD3D9);
}


void Server::RenderDebugString(const char* szText)
{
	
	m_pNewFont->DrawText(0,0,szText);
}

void Server::LoadHLSL(const char* szFileName)
{


	if (!szFileName)
	{
		return;
	}


	// ★셰이더 읽기
	HRESULT hr;
	LPD3DXBUFFER pErr=NULL;
	if( FAILED( hr = D3DXCreateEffectFromFile(m_pD3DDevice, szFileName, NULL, NULL,D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION , NULL, &m_pEffect, &pErr )))
	{
		MessageBox( NULL, (LPCTSTR)pErr->GetBufferPointer(), "ERROR", MB_OK);
		DXTrace(__FILE__, __LINE__, hr, _T("Error"), TRUE);
		return;
	}
	
	D3DXEFFECT_DESC desc;
	hr = m_pEffect->GetDesc(&desc);
	
	m_hTPhong = m_pEffect->GetTechniqueByName( _T("TPhong") );
	m_hTPhongDiffuse = m_pEffect->GetTechniqueByName( _T("TPhongDiffuse") );
	m_hTPhongDiffuseLight = m_pEffect->GetTechniqueByName( _T("TPhongDiffuseLight") );	
	m_hTPhongDiffuseBump = m_pEffect->GetTechniqueByName( _T("TPhongDiffuseBump") );
	
	m_hTSkinningPhong = m_pEffect->GetTechniqueByName( _T("TSkinningPhong") );	
	m_hTSkinningPhongDiffuse = m_pEffect->GetTechniqueByName( _T("TSkinningPhongDiffuse") );	
	m_hmWorld = m_pEffect->GetParameterByName( NULL, "gWorldMatrix" );
	m_hmView = m_pEffect->GetParameterByName( NULL, "gViewMatrix" );
	m_hmProjection = m_pEffect->GetParameterByName( NULL, "gProjectionMatrix" );
	m_hmViewProjection = m_pEffect->GetParameterByName( NULL, "gViewProjectionMatrix" );
	m_hmPalette = m_pEffect->GetParameterByName( NULL, "Palette" );


	m_hvWorldLightPosition = m_pEffect->GetParameterByName( NULL, "gWorldLightPosition" );
	m_hvWorldCameraPosition = m_pEffect->GetParameterByName( NULL, "gWorldCameraPosition" );

	D3DXVECTOR4 posLight(0.0f,5000.0f,-5000.0f,0.0f);
	m_pEffect->SetVector(m_hvWorldLightPosition,&posLight);

	SAFE_RELEASE(pErr);


	std::bitset<Material::MAX> indexRenderQueue;

	m_listRenderQueue[indexRenderQueue.to_ulong()].m_hTechnique = m_hTPhong;

	indexRenderQueue.set(Material::DIFFUSE);
	m_listRenderQueue[indexRenderQueue.to_ulong()].m_hTechnique = m_hTPhongDiffuse;

	indexRenderQueue.set(Material::NORMAL);
	m_listRenderQueue[indexRenderQueue.to_ulong()].m_hTechnique = m_hTPhongDiffuseBump;

	indexRenderQueue.reset(Material::NORMAL);
	indexRenderQueue.set(Material::LIGHT);
	m_listRenderQueue[indexRenderQueue.to_ulong()].m_hTechnique = m_hTPhongDiffuseLight;

	for (int i=0;i<16;i++)
	{	
		if (m_listRenderQueue[i].m_hTechnique == NULL )	
			m_listRenderQueue[i].m_hTechnique = m_hTPhongDiffuse;
	}

	indexRenderQueue.reset();
	m_listRenderQueueSkinned[indexRenderQueue.to_ulong()].m_hTechnique = m_hTSkinningPhong;
	for (int i=0;i<16;i++)
	{	
		if (m_listRenderQueueSkinned[i].m_hTechnique == NULL )	
			m_listRenderQueueSkinned[i].m_hTechnique = m_hTSkinningPhongDiffuse;
	}


}

void Server::Render()
{
	for (int i=0;i<16;i++)
	{
		// 여기선 렌더큐들만 그린다
		UINT passes = 0;
		m_pEffect->SetTechnique(m_listRenderQueue[i].m_hTechnique);
		m_pEffect->Begin(&passes, 0);
		m_pEffect->BeginPass(0);
		// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다

		m_listRenderQueue[i].Render();

		m_pEffect->EndPass();
		m_pEffect->End();
	}

	for (int i=0;i<16;i++)
	{
		// 여기선 렌더큐들만 그린다
		UINT passes = 0;
		m_pEffect->SetTechnique(m_listRenderQueueSkinned[i].m_hTechnique);
		m_pEffect->Begin(&passes, 0);
		m_pEffect->BeginPass(0);
		// 쉐이더 설정은 꼭 Begin전에 한다. 따라서 쉐이더별로 정렬이 필요하다

		m_listRenderQueueSkinned[i].Render();

		m_pEffect->EndPass();
		m_pEffect->End();

	}


}

void Server::Begin()
{
	m_pD3DDevice->SetViewport(&m_viewPortInfo);
	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
	m_pD3DDevice->BeginScene();	
}

void Server::End()
{
	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present( NULL, NULL, NULL, NULL );	
}

}
