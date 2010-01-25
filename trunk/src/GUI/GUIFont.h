#pragma once
#include "../D3D9/SharedDevice.h"

#include <d3dx9core.h>

class cGUIFont:
	private SharedLPDIRECT3DDEVICE9
{
public:
	cGUIFont(void);
	virtual ~cGUIFont(void);
private:
	LPD3DXFONT			m_pD3DXFont;
	RECT				m_DestRect;
	D3DXCOLOR			m_Color;
	DWORD				m_Format;

public:
	DWORD				GetFormat() const { return m_Format; }
	void				SetFormat(DWORD val) { m_Format = val; }
	D3DXCOLOR&			GetColor()  { return m_Color; }
	void				SetColor(D3DXCOLOR& val) { m_Color = val; }
	
	void				DrawText(UINT x,UINT y,LPCTSTR text);

	void				Create();
};
