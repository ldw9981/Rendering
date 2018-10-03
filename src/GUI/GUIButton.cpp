#include "StdAfx.h"
#include "GUIButton.h"
#include "GUIBase.h"
#include "Graphics/Graphics.h"
#include "Graphics/RscTexture.h"
#include "Resource/ResourceMng.h"
#include "Foundation/Define.h"
#include "Input/Input.h"
#include "CursorManager.h"
#include "Graphics/World.h"

namespace Sophia
{


cGUIButton::cGUIButton()
{
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
	if (g_pInput->Mouse_IsMove())
	{
		POINT pt;
		CursorManager::GetInstance()->GetCursorPos(&pt);
		if (!PtInRect(&m_rect,pt))	
			m_ImageIndex=0;
		else 
			m_ImageIndex=1;	
	}
	if(g_pInput->Mouse_IsTurnDn(0))
	{		
		POINT pt;
		CursorManager::GetInstance()->GetCursorPos(&pt);
		if (PtInRect(&m_rect,pt))
		{
			GetMediator()->Notify(this,WM_LBUTTONDOWN,0,0);				
		}
	}	
}

void cGUIButton::Control()
{

}

void cGUIButton::Render(DWORD elapseTime)
{
	Graphics::m_pDevice->SetTexture (0, m_pImage->GetD3DTexture() );
	Graphics::m_pDevice->SetFVF(FVF_GUIVERTEX);
	Graphics::m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, &m_pButtonFan[m_ImageIndex], sizeof(GUIVERTEX));
}

void cGUIButton::Create( const char* strImageFile)
{	
	m_ImageNumber=2;

	m_pImage= cResourceMng::m_pInstance->CreateRscTexture(strImageFile);
	m_pImage->AddRef();

	D3DSURFACE_DESC desc;
	m_pImage->GetD3DTexture()->GetLevelDesc(0,&desc);
	
	m_Width = (float)desc.Width/ (float)m_ImageNumber;	
	m_Height = (float)desc.Height;
	m_TWidth = 1.0f / 2.0f;	

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = (LONG)m_Width;
	m_rect.bottom = (LONG)m_Height;

	assert(m_pButtonFan==NULL);
	m_pButtonFan = new BUTTONFAN[m_ImageNumber];	
	SetPos(0,0);
}

void cGUIButton::SetPos( UINT x,UINT y )
{	
	if (m_pWorld)
	{
		D3DVIEWPORT9& viewPort = m_pWorld->m_ViewPortInfo;
		m_rect.left = viewPort.X + x;
		m_rect.top = viewPort.Y + y;
		m_rect.right = viewPort.X + (LONG)m_Width + x;
		m_rect.bottom = viewPort.Y + (LONG)m_Height + y;	
	}
	else
	{
		m_rect.left = 0;
		m_rect.top = 0;
		m_rect.right = (LONG)m_Width + x;
		m_rect.bottom = (LONG)m_Height + y;	
	}
	// 이미지의 위치, 사이즈로 영역 계산


	// vertex의 수정
	for(UINT i=0;i<m_ImageNumber;i++)
	{
		m_pButtonFan[i].vertices[0].vertex = D3DXVECTOR4( (float)m_rect.left,(float)m_rect.top,0.0f,1.0f);		
		m_pButtonFan[i].vertices[0].tex.u = i*m_TWidth + 0.0f;
		m_pButtonFan[i].vertices[0].tex.v = 0.0f;

		m_pButtonFan[i].vertices[1].vertex = D3DXVECTOR4( (float)m_rect.right,(float)m_rect.top,0.0f,1.0f);		
		m_pButtonFan[i].vertices[1].tex.u = i*m_TWidth + m_TWidth;
		m_pButtonFan[i].vertices[1].tex.v = 0.0f;

		m_pButtonFan[i].vertices[2].vertex = D3DXVECTOR4( (float)m_rect.right,(float)m_rect.bottom,0.0f,1.0f);		
		m_pButtonFan[i].vertices[2].tex.u = i*m_TWidth + m_TWidth;
		m_pButtonFan[i].vertices[2].tex.v = 1.0f;

		m_pButtonFan[i].vertices[3].vertex = D3DXVECTOR4( (float)m_rect.left,(float)m_rect.bottom,0.0f,1.0f);		
		m_pButtonFan[i].vertices[3].tex.u = i*m_TWidth + 0.0f;
		m_pButtonFan[i].vertices[3].tex.v = 1.0f;				
	}
}

}