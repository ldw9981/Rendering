#include "StdAfx.h"
#include "DebugInfoScene.h"
#include "../GUI/GUIFont.h"
#include "../Framework/D3DFramework.h"


cDebugInfoScene::cDebugInfoScene()
{
	m_pFont = new cGUIFont;
	m_bRender = FALSE;
	SetViewPortInfo(0,0,
		g_pD3DFramework->GetRequestRectWidth(),
		g_pD3DFramework->GetRequestRectHeight());
}

cDebugInfoScene::~cDebugInfoScene()
{
	SAFE_DELETE(m_pFont);
}

void cDebugInfoScene::Open( void* arg )
{
	
}

void cDebugInfoScene::Close()
{

}

void cDebugInfoScene::Control()
{
	if (m_pWinInput->IsTurnDn('I'))
	{
		m_bRender = !m_bRender;
	}
}

void cDebugInfoScene::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{

}

void cDebugInfoScene::Update( DWORD elapseTime )
{
	m_DebugString.clear();
#ifdef _DEBUG
	m_DebugString=L"DEBUG MODE\n";
#endif

}

void cDebugInfoScene::ProcessRender()
{
	SetViewPort();
	if (m_bRender)
	{
		m_pFont->DrawText(0,0,m_DebugString.c_str());
	}	
}

void cDebugInfoScene::AddDebugString( LPCTSTR format, ... )
{
	va_list argptr;
	TCHAR    buf[1024];
	int i;
	memset(buf, 0, sizeof(buf));

	va_start(argptr, format);	
	i = _vstprintf_s(buf, format, argptr);
	va_end(argptr);

	assert(i<1024 && _T("����� �޽��� �Ѹ��� �ʹ�������"));

	if (buf[i-1] == '\n' )
	{
		buf[i-1] = '\r';
		buf[i]   = '\n';
		buf[i+1] = 0;
	}
	m_DebugString += buf;

}