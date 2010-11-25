#pragma once

#include "D3D9Server/RscVertexBuffer.h"
#include "Foundation/Interface.h"
#include "WinInput/WinInput.h"
#include "GUIBase.h"
#include "Resource/ResourceMng.h"

struct GUIVERTEX
{
	//x,y,z
	D3DXVECTOR4	vertex;
	D3DCOLOR	color;	
	TEXCOORD	tex;
};

struct BUTTONFAN
{
	GUIVERTEX vertices[4];
};

#define FVF_GUIVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)  

class cRscTexture;
class INotifiable;
class cGUIButton:
	public cGUIBase,
	private cStaticResourceMng

{
public:
	cGUIButton(INotifiable* pMediator);
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
	void				Create(const char* strImageFile);
	virtual void		Render();
	virtual void		Update(DWORD elapsetime);
	virtual void		Control();
};
