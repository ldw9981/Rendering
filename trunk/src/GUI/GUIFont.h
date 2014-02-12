#pragma once

class World;
class cGUIFont
{
public:
	cGUIFont(void);
	virtual ~cGUIFont(void);
private:
	LPD3DXFONT			m_pD3DXFont;
	RECT				m_rect;
	D3DXCOLOR			m_Color;
	DWORD				m_Format;
	World*				m_pWorld;
	
public:
	void SetWorld(World* val);
	DWORD				GetFormat() const { return m_Format; }
	void				SetFormat(DWORD val) { m_Format = val; }
	D3DXCOLOR&			GetColor()  { return m_Color; }
	void				SetColor(D3DXCOLOR& val) { m_Color = val; }
	
	void				DrawText(UINT x,UINT y,const char* text);

	void				Create();
};
