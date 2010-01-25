#include "TestUI.h"

struct UIVERTEX
{
	float x,y,z,w;
	D3DCOLOR	color;	
	float		u;  
	float		v;
};
#define FVF_UIVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)  

UIVERTEX g_UIVertices[]=
{
	{    0.0f,   0.0f , 0.0f,1.0f , 0xFFFFFFFF ,	0.0f,0.0f },
	{  250.0f,   0.0f , 0.0f,1.0f , 0xFFFFFFFF ,	1.0f,0.0f },
	{  250.0f, 250.0f , 0.0f,1.0f , 0xFFFFFFFF ,	1.0f,1.0f },
	{    0.0f, 250.0f , 0.0f,1.0f , 0xFFFFFFFF ,	0.0f,1.0f }	
};
cTestUI::cTestUI()
{
	m_pTexture=NULL;
}
cTestUI::~cTestUI(void)
{	
	SAFE_RELEASE(m_pTexture);
}
void cTestUI::Init()
{
	D3DXCreateTextureFromFile(m_pD3DDevice9,_T("ground.bmp"),&m_pTexture);
}
void cTestUI::Uninit()
{

}

void cTestUI::Update(DWORD elapseTime)
{

}

void cTestUI::Render()
{		
	
	m_pD3DDevice9->SetTexture (0, m_pTexture );
	m_pD3DDevice9->SetFVF(FVF_UIVERTEX);
	m_pD3DDevice9->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, g_UIVertices, sizeof(UIVERTEX));
}

void cTestUI::ProcessRender()
{
	Render();
}