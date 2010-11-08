#include "StdAfx.h"
#include "TestGameApp.h"
#include "ObjDragon.h"
#include "Ground.h"
#include "BackWall.h"
#include "./Renderer/Camera.h"
#include "./Resource/ResourceMng.h"
#include "./Resource/RscTexture.h"


cTestGameApp::cTestGameApp( LPCWSTR szTitleName,BOOL bFullScreen,int nWidth,int nHeight)
:cD3DFramework(szTitleName,bFullScreen,nWidth,nHeight)
{
	
}

cTestGameApp::~cTestGameApp(void)
{
}

void cTestGameApp::OnInitApplication()
{

	m_pDragon = new cObjDragon(NULL);
	m_pDragon->LoadModel(_T("largeball.ASE"));
	m_pDragon->Init();
	AddObject(m_pDragon);

	//Projection	
	GetRenderer()->GetDefaultCamera()->SetPerspective(D3DXToRadian(45),1.0f,10000.0f,(float)GetClientWidth(),(float)GetClientHeight());	
	//View
	GetRenderer()->GetDefaultCamera()->SetLookAt(&D3DXVECTOR3(0.0f,0.0f, -950.0f),&D3DXVECTOR3(0.0f, 0.0f, 0.0f),&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		
	
	
}

void cTestGameApp::OnCloseApplication()
{


	m_pDragon->Uninit();
	delete m_pDragon;
}


BOOL cTestGameApp::OnWM_Keyboard( MSG& msg )
{
	return FALSE;
}


BOOL cTestGameApp::OnWM_Mouse( MSG& msg )
{
	return FALSE;
}

BOOL cTestGameApp::OnWM_General( MSG& msg )
{
	switch(msg.message)
	{
	case WM_ACTIVATEAPP:
		if (!msg.wParam)
		{
			//MessageBox(NULL,L"the window is being deactivated.",L"알림",MB_OK);
		}
		else
		{
			//MessageBox(NULL,L"the window is being activated.",L"알림",MB_OK);
		}
		return TRUE;
	
	}
	return FALSE;
}



int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	cTestGameApp TestGameApp(L"EnviromentMapping",FALSE,1024,768);
	TestGameApp.Run();
	return 0;
}