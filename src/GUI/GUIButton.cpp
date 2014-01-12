#include "StdAfx.h"
#include "GUIButton.h"
#include "GUIBase.h"
#include "Graphics/Graphics.h"
#include "Graphics/RscTexture.h"
#include "Resource/ResourceMng.h"
#include "Foundation/Define.h"
#include "Input/Input.h"

cGUIButton::cGUIButton(INotifiable* pMediator)
{
	SetMediator(pMediator);
	m_pImage=NULL;
	m_pButtonFan=NULL;
	m_ImageIndex=0;
	m_TWidth=0.0f;
	m_Pushed=FALSE;
}

cGUIButton::~cGUIButton(void)
{
	SAFE_RELEASE(m_pImage);
	SAFE_DELETEARRAY(m_pButtonFan);
}


void cGUIButton::Update( DWORD elapsetime )
{
	POINT pt;
	g_pInput->GetMouseLocation(pt.x,pt.y);
	if (!PtInRect(&m_Rect,pt))	m_ImageIndex=0;
	else m_ImageIndex=1;	
}

void cGUIButton::Control()
{
	if(g_pInput->Mouse_IsTurnDn(0))
	{		
		POINT pt;
		g_pInput->GetMouseLocation(pt.x,pt.y);
		if (PtInRect(&m_Rect,pt))
		{
			GetMediator()->Notify(this,WM_LBUTTONDOWN,0,0);				
		}
	}	
}

void cGUIButton::Render()
{
	Graphics::m_pDevice->SetTexture (0, m_pImage->GetD3DTexture() );
	Graphics::m_pDevice->SetFVF(FVF_GUIVERTEX);
	Graphics::m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, &m_pButtonFan[m_ImageIndex], sizeof(GUIVERTEX));
}

void cGUIButton::Create( const char* strImageFile)
{	
	m_ImageNumber=2;

	m_pImage= cResourceMng::m_pResourceMng->CreateRscTexture(strImageFile);
	m_pImage->AddRef();

	D3DSURFACE_DESC desc;
	m_pImage->GetD3DTexture()->GetLevelDesc(0,&desc);
	
	m_Width = (float)desc.Width/ (float)m_ImageNumber;	
	m_Height = (float)desc.Height;
	m_TWidth = 1.0f / 2.0f;	

	m_Rect.left = 0;
	m_Rect.top = 0;
	m_Rect.right = (LONG)m_Width;
	m_Rect.bottom = (LONG)m_Height;

	assert(m_pButtonFan==NULL);
	m_pButtonFan = new BUTTONFAN[m_ImageNumber];	
	SetPos(0,0);
}

void cGUIButton::SetPos( UINT x,UINT y )
{
	// ViewPort영역에서 시작
 	D3DVIEWPORT9 ViewPort;
 	Graphics::m_pDevice->GetViewport(&ViewPort);
 	m_Rect.left = ViewPort.X;
 	m_Rect.top = ViewPort.Y;
 	m_Rect.right = ViewPort.X;
 	m_Rect.bottom = ViewPort.Y;

	// 이미지의 위치, 사이즈로 영역 계산
	m_Rect.left += x;
	m_Rect.top += y;
	m_Rect.right += (LONG)m_Width + x;
	m_Rect.bottom += (LONG)m_Height + y;	

	// vertex의 수정
	for(UINT i=0;i<m_ImageNumber;i++)
	{
		m_pButtonFan[i].vertices[0].vertex = D3DXVECTOR4( (float)m_Rect.left,(float)m_Rect.top,0.0f,1.0f);		
		m_pButtonFan[i].vertices[0].tex.u = i*m_TWidth + 0.0f;
		m_pButtonFan[i].vertices[0].tex.v = 0.0f;

		m_pButtonFan[i].vertices[1].vertex = D3DXVECTOR4( (float)m_Rect.right,(float)m_Rect.top,0.0f,1.0f);		
		m_pButtonFan[i].vertices[1].tex.u = i*m_TWidth + m_TWidth;
		m_pButtonFan[i].vertices[1].tex.v = 0.0f;

		m_pButtonFan[i].vertices[2].vertex = D3DXVECTOR4( (float)m_Rect.right,(float)m_Rect.bottom,0.0f,1.0f);		
		m_pButtonFan[i].vertices[2].tex.u = i*m_TWidth + m_TWidth;
		m_pButtonFan[i].vertices[2].tex.v = 1.0f;

		m_pButtonFan[i].vertices[3].vertex = D3DXVECTOR4( (float)m_Rect.left,(float)m_Rect.bottom,0.0f,1.0f);		
		m_pButtonFan[i].vertices[3].tex.u = i*m_TWidth + 0.0f;
		m_pButtonFan[i].vertices[3].tex.v = 1.0f;				
	}
}