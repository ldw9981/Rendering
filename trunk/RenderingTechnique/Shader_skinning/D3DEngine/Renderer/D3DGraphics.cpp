#include "StdAfx.h"
#include "D3DGraphics.h"
#include "Camera.h"
#include "../Shader.h"

cD3DGraphics* cStaticD3DGraphics::m_pGraphics=NULL;

cD3DGraphics::cD3DGraphics()
{
	m_pGraphics=this;
	m_pD3D9 = NULL;
	m_pD3DDevice9 = NULL;
	m_pFont=NULL;
	m_pDefaultTopCamera=NULL;
	memset(&m_D3DPP,0,sizeof(m_D3DPP));
}

cD3DGraphics::~cD3DGraphics(void)
{
	SAFE_DELETE(m_pDefaultTopCamera);
	if( m_pFont != NULL )
		m_pFont->Release();

	SAFE_RELEASE(m_pD3DDevice9);
	SAFE_RELEASE(m_pD3D9);
}


void cD3DGraphics::RenderDebugString()
{
	//
	// Render some text using our new font...
	//
	RECT destRect1;
	SetRect( &destRect1,0,0, 0, 0 );
	m_pFont->DrawText( NULL,m_DebugString.c_str(), -1, &destRect1, DT_NOCLIP, 
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) );
}

void cD3DGraphics::AddDebugString(LPCTSTR format, ...)
{
	va_list argptr;
	TCHAR    buf[1024];
	int i;
	memset(buf, 0, sizeof(buf));

	va_start(argptr, format);	
	i = _vstprintf_s(buf, format, argptr);
	va_end(argptr);

	assert(i<1024 && _T("디버그 메시지 뿌릴께 너무많은데"));

	if (buf[i-1] == '\n' )
	{
		buf[i-1] = '\r';
		buf[i]   = '\n';
		buf[i+1] = 0;
	}
	m_DebugString += buf;

}

//---------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//---------------------------------------------
void cD3DGraphics::InitD3D()
{
	// 1) D3D를 생성한다.
	m_pD3D9 = Direct3DCreate9( D3D_SDK_VERSION );

	// 2) Device를 생성을 위한 Parameter를 설정한다.
	
	m_D3DPP.Windowed	 = TRUE;
	m_D3DPP.SwapEffect	 = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.BackBufferFormat	 = D3DFMT_UNKNOWN;

	// Zbuffer사용
	m_D3DPP.EnableAutoDepthStencil	= TRUE;		
	m_D3DPP.AutoDepthStencilFormat	= D3DFMT_D16;


	// 3) Device를 생성한다.

	m_pD3D9->CreateDevice( 
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,		
		&m_D3DPP, 
		&m_pD3DDevice9 );




	m_pD3DDevice9->SetRenderState(D3DRS_ZENABLE,TRUE);

	SetupLight();
	InitFont();
	m_pDefaultTopCamera = new cCamera();
	m_pDefaultTopCamera->Init();
	g_pD3DDevice9=m_pD3DDevice9;
	LoadVertexShader(L"skinning.vs");

}

void cD3DGraphics::SetupLight()
{
	
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

	

	m_pD3DDevice9->SetMaterial(&mtrl);


	D3DLIGHT9 light;
	ZeroMemory(&light,sizeof(D3DLIGHT9));
	light.Type	=	D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	light.Direction = D3DXVECTOR3(0.0f,-1.0f,1.0f);		// 타겟 방향
	//	light.Range = 1000.0f;

	m_pD3DDevice9->SetLight(0,&light);
	m_pD3DDevice9->LightEnable(0,TRUE);
	m_pD3DDevice9->SetRenderState(D3DRS_LIGHTING,TRUE);
//	m_pD3DDevice9->SetRenderState(D3DRS_AMBIENT,0x00202020);

}

void cD3DGraphics::InitFont()
{
	//
	// To create a Windows friendly font using only a point size, an 
	// application must calculate the logical height of the font.
	// 
	// This is because functions like CreateFont() and CreateFontIndirect() 
	// only use logical units to specify height.
	//
	// Here's the formula to find the height in logical pixels:
	//
	//             -( point_size * LOGPIXELSY )
	//    height = ----------------------------
	//                          72
	//

	HRESULT hr;
	HDC hDC;
	int nHeight;
	int nPointSize = 12;

	hDC = GetDC( NULL );

	nHeight = -( MulDiv( nPointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72 ) );

	ReleaseDC( NULL, hDC );

	// Create a font for statistics and help output
	hr = D3DXCreateFont( m_pD3DDevice9, nHeight, 0, FW_BOLD, 0, FALSE, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), 
		&m_pFont );

	if( FAILED( hr ) )
		MessageBox(NULL,_T("Call to D3DXCreateFont failed!"), _T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
}



cCamera* cD3DGraphics::GetDefaultTopCamera()
{
	assert(m_pDefaultTopCamera!=NULL);
	return m_pDefaultTopCamera;
}

void cD3DGraphics::ClearDebugString()
{
#ifdef _DEBUG
	m_DebugString=_T("DEBUG MODE\n");
#endif
}

void cD3DGraphics::Update(DWORD elapseTime)
{		
	ClearDebugString();
}

void cD3DGraphics::Render()
{
	GetDefaultTopCamera()->Render();
	RenderDebugString();
}

void cD3DGraphics::RenderStart()
{
	// 1) Render Target을 지운다.
	m_pD3DDevice9->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
	// 2) Begin the scene
	m_pD3DDevice9->BeginScene();
}

void cD3DGraphics::RenderEnd()
{
	m_pDefaultTopCamera->Render();

	RenderDebugString();
	m_DebugString=_T("");
	// 4) End the scene
	m_pD3DDevice9->EndScene();
	// 5) Present한다.
	m_pD3DDevice9->Present( NULL, NULL, NULL, NULL );	
}