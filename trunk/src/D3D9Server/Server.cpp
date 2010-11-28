#include "StdAfx.h"
#include "Server.h"
#include "Foundation/Define.h"
#include "GUI/GUIFont.h"

namespace D3D9
{


Server::Server(void)
{
}

Server::~Server(void)
{
}

bool Server::Init()
{
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

	m_pD3DDevice->GetViewport(&m_BaseViewPort);

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



	
	return true;
}

void Server::Uninit()
{
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
	/*
	if (!szFileName)
	{
		return;
	}
	// ★셰이더 읽기
	HRESULT hr;
	LPD3DXBUFFER pErr=NULL;
	if( FAILED( hr = D3DXCreateEffectFromFile(m_pD3DDevice, szFileName, NULL, NULL,0 , NULL, &m_pEffect, &pErr )))
	{
		// 셰이더 읽기 실패
		MessageBox( NULL, (LPCTSTR)pErr->GetBufferPointer(), "ERROR", MB_OK);
	}
	SAFE_RELEASE(pErr);
	*/
}

}
