#pragma once

#include "Graphics/Vertex.h"
#include "Foundation/Interface.h"
#include "Input/Input.h"
#include "GUIBase.h"




struct BUTTONFAN
{
	GUIVERTEX vertices[4];
};



class cRscTexture;
class INotifiable;
class cGUIButton:
	public cGUIBase

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
