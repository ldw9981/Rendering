#pragma once

#include "GUIBase.h"
#include "../D3D9/VertexBuffer.h"
#include "../Framework/Interface.h"
#include "../Framework/Input/WinInput.h"
#include "../Framework/App/App.h"
#include "../Foundation/ResourceMng.h"

namespace Application
{
	class App;
}
namespace Framework
{
	class INotifiable;
}

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

class Texture;



class cGUIButton:
	public cGUIBase,
	public SharedResourceMng

{
public:
	cGUIButton(Framework::INotifiable* pMediator);
	virtual ~cGUIButton(void);

public:

private:
	BUTTONFAN*		    m_pButtonFan;	
	UINT 				m_ImageNumber;
	UINT 				m_ImageIndex;
	Texture*			m_pImage;
	
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



