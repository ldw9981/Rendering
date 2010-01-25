#include "../D3D9/SharedDevice.h"
#include "../Foundation/Define.h"
#include "GUIFont.h"

cGUIFont::cGUIFont(void)
{
	memset(&m_DestRect,0,sizeof(m_DestRect));

	m_Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Format=DT_NOCLIP;
	m_pD3DXFont=NULL;
	Create();
}

cGUIFont::~cGUIFont(void)
{
	SAFE_RELEASE(m_pD3DXFont);
}

void cGUIFont::DrawText(UINT x,UINT y, LPCTSTR text )
{
	m_DestRect.left = x;
	m_DestRect.top = y;
	m_DestRect.right = 0;
	m_DestRect.bottom = 0;

	m_pD3DXFont->DrawText( NULL,text, -1,&m_DestRect, m_Format, m_Color );
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

	hDC = GetDC( NULL );

	nHeight = -( MulDiv( nPointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72 ) );

	ReleaseDC( NULL, hDC );

	// Create a font for statistics and help output
	hr = D3DXCreateFont( m_SharedLPDIRECT3DDEVICE9, nHeight, 0, FW_BOLD, 0, FALSE, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), 
		&m_pD3DXFont );
}