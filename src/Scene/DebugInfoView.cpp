#include "StdAfx.h"
#include "DebugInfoView.h"
#include "GUI/GUIFont.h"
#include "Framework/D3DFramework.h"
#include "Foundation/Define.h"

cDebugInfoView* cDebugInfoView::m_pInstance=0;

cDebugInfoView::cDebugInfoView()
{
	m_pFont = new cGUIFont;
	m_bRender = true;
	SetViewPortInfo(0,0,
		g_pApp->GetRequestRectWidth(),
		g_pApp->GetRequestRectHeight());
	m_pInstance = this;
}

cDebugInfoView::~cDebugInfoView()
{
	SAFE_DELETE(m_pFont);
}

void cDebugInfoView::Enter( void* arg )
{
	
}

void cDebugInfoView::Leave()
{

}

void cDebugInfoView::Control()
{
	if (m_pWinInput->IsTurnDn('I'))
	{
		m_bRender = !m_bRender;
	}
}

void cDebugInfoView::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{

}

void cDebugInfoView::Update( DWORD elapseTime )
{
	m_DebugString.clear();
#ifdef _DEBUG
	m_DebugString="DEBUG MODE\n";
#endif

}

void cDebugInfoView::ProcessRender()
{
	//SetViewPort();
	if (m_bRender)
	{
		m_pFont->DrawText(0,0,m_DebugString.c_str());
	}	
}

void cDebugInfoView::AddDebugString( const char* format, ... )
{
	/*
	va_list argptr;
	char    buf[1024];
	int i;
	memset(buf, 0, sizeof(buf));

	va_start(argptr, format);	
	i = _vstprintf_s(buf, format, argptr);
	va_end(argptr);

	assert(i<1024 && "디버그 메시지 뿌릴께 너무많은데");

	if (buf[i-1] == '\n' )
	{
		buf[i-1] = '\r';
		buf[i]   = '\n';
		buf[i+1] = 0;
	}
	m_DebugString += buf;
	*/

}

