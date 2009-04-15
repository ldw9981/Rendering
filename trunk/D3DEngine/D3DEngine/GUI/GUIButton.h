#pragma once

#include "../Resource/RscVertexBuffer.h"
#include "../Common/Interface.h"
#include "../Input/WinInput.h"
#include "GUIBase.h"

struct GUIVERTEX
{
	enum { FVF=(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) };
	//x,y,z
	D3DXVECTOR4	vertex;
	D3DCOLOR	color;	
	TEXCOORD	tex;
};

struct BUTTONFAN
{
	GUIVERTEX vertices[4];
};


class cScene;
class cRscTexture;
class INotifiable;
class cGUIButton:
	public cGUIBase
{
public:
	cGUIButton(cScene* pMediator);
	virtual ~cGUIButton(void);

private:
	
	BUTTONFAN*		    m_pButtonFan;	
	UINT 				m_ImageNumber;
	UINT 				m_ImageIndex;
	cRscTexture*		m_pImage;
	
	RECT				m_Rect;
	float				m_Width;
	float				m_Height;
	float				m_TWidth;
	BOOL				m_Pushed;
	
	
public:
	void				SetPos(UINT x,UINT y);
	void				Create(LPCTSTR strImageFile);
	virtual void		Render();
	virtual void		Update(DWORD elapsetime);
	virtual void		Control();
};
