#include "StdAfx.h"
#include "GUIButton.h"
#include "GUIBase.h"
#include "D3D9Server/RscTexture.h"
#include "Framework/D3DFramework.h"
#include "Foundation/Define.h"

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
	POINT pt=m_pWinInput->GetMousePoint();
	if (!PtInRect(&m_Rect,pt))	m_ImageIndex=0;
	else m_ImageIndex=1;	
}

void cGUIButton::Control()
{
	if(m_pWinInput->IsTurnDn(VK_LBUTTON))
	{		
		POINT pt=m_pWinInput->GetMousePoint();
		if (PtInRect(&m_Rect,pt))
		{
			GetMediator()->Notify(this,WM_LBUTTONDOWN,0,0);				
		}
	}	
}

void cGUIButton::Render()
{
	m_pD3DDevice->SetTexture (0, m_pImage->GetD3DTexture() );
	m_pD3DDevice->SetFVF(FVF_GUIVERTEX);
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, &m_pButtonFan[m_ImageIndex], sizeof(GUIVERTEX));
}

void cGUIButton::Create( const char* strImageFile)
{	
	m_ImageNumber=2;
	m_pImage=m_ResourceMng.CreateRscTexture(strImageFile);
	
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
	// ViewPort�������� ����
 	D3DVIEWPORT9 ViewPort;
 	m_pD3DDevice->GetViewport(&ViewPort);
 	m_Rect.left = ViewPort.X;
 	m_Rect.top = ViewPort.Y;
 	m_Rect.right = ViewPort.X;
 	m_Rect.bottom = ViewPort.Y;

	// �̹����� ��ġ, ������� ���� ���
	m_Rect.left += x;
	m_Rect.top += y;
	m_Rect.right += (LONG)m_Width + x;
	m_Rect.bottom += (LONG)m_Height + y;	

	// vertex�� ����
	for(UINT i=0;i<m_ImageNumber;i++)
	{
		m_pButtonFan[i].vertices[0].vertex = D3DXVECTOR4( (float)m_Rect.left,(float)m_Rect.top,0.0f,1.0f);		
		m_pButtonFan[i].vertices[0].color = 0xFFFFFFFF;
		m_pButtonFan[i].vertices[0].tex.first = i*m_TWidth + 0.0f;
		m_pButtonFan[i].vertices[0].tex.second = 0.0f;

		m_pButtonFan[i].vertices[1].vertex = D3DXVECTOR4( (float)m_Rect.right,(float)m_Rect.top,0.0f,1.0f);		
		m_pButtonFan[i].vertices[1].color = 0xFFFFFFFF;
		m_pButtonFan[i].vertices[1].tex.first = i*m_TWidth + m_TWidth;
		m_pButtonFan[i].vertices[1].tex.second = 0.0f;

		m_pButtonFan[i].vertices[2].vertex = D3DXVECTOR4( (float)m_Rect.right,(float)m_Rect.bottom,0.0f,1.0f);		
		m_pButtonFan[i].vertices[2].color = 0xFFFFFFFF;
		m_pButtonFan[i].vertices[2].tex.first = i*m_TWidth + m_TWidth;
		m_pButtonFan[i].vertices[2].tex.second = 1.0f;

		m_pButtonFan[i].vertices[3].vertex = D3DXVECTOR4( (float)m_Rect.left,(float)m_Rect.bottom,0.0f,1.0f);		
		m_pButtonFan[i].vertices[3].color = 0xFFFFFFFF;
		m_pButtonFan[i].vertices[3].tex.first = i*m_TWidth + 0.0f;
		m_pButtonFan[i].vertices[3].tex.second = 1.0f;				
	}
}