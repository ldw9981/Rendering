#include "StdAfx.h"
#include "GUIFont.h"
#include "Graphics/World.h"
#include "Graphics/Graphics.h"
#include "Foundation/Define.h"

namespace Sophia
{

cGUIFont::cGUIFont(void)
{
	m_pWorld = NULL;
	memset(&m_rect,0,sizeof(m_rect));

	m_Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Format=DT_NOCLIP;
	m_pD3DXFont=NULL;
	Create();
}

cGUIFont::~cGUIFont(void)
{
	SAFE_RELEASE(m_pD3DXFont);
}

void cGUIFont::DrawText(UINT x,UINT y, const char* text )
{	
	if (m_pWorld)
	{
		D3DVIEWPORT9& viewPort = m_pWorld->m_ViewPortInfo;
		m_rect.left = viewPort.X + x;
		m_rect.top = viewPort.Y + y;
		m_rect.right = 0;
		m_rect.bottom = 0;
	}
	else
	{
		m_rect.left = x;
		m_rect.top = y;
		m_rect.right = 0;
		m_rect.bottom = 0;
	}

	HRESULT hr = m_pD3DXFont->DrawText( NULL,text, -1,&m_rect, m_Format, m_Color );

	if( FAILED( hr ) )
		MessageBox(NULL,"Call to D3DXCreateFont failed!", "ERROR",MB_OK|MB_ICONEXCLAMATION);
}

void cGUIFont::Create()
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

	hDC = GetDC( Graphics::m_pInstance->m_hWnd );

	nHeight = -( MulDiv( nPointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72 ) );

	ReleaseDC( NULL, hDC );

	// Create a font for statistics and help output
	hr = D3DXCreateFont( Graphics::m_pDevice, nHeight, 0, FW_BOLD, 0, FALSE, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Aria"), 
		&m_pD3DXFont );
	
}

void cGUIFont::SetWorld( World* val )
{
	m_pWorld = val;
}

}