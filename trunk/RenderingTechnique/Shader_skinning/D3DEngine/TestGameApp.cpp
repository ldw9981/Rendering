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
	m_pGround = new cGround(NULL);
	m_pGround->Init();
	AddObject(m_pGround);

	m_pBackWall = new cBackWall(NULL);
	m_pBackWall->Init();
	AddObject(m_pBackWall);


	m_pDragon = new cObjDragon(NULL);
//	m_pDragon->LoadModel(_T("03ik-joe.ASE"));
	m_pDragon->LoadModel(_T("dragon2.ASE"));
	m_pDragon->Init();
	AddObject(m_pDragon);

	//Projection	
	GetRenderer()->GetDefaultTopCamera()->SetPerspective(D3DXToRadian(45),1.0f,10000.0f,(float)GetClientWidth(),(float)GetClientHeight());	
	//View
	GetRenderer()->GetDefaultTopCamera()->SetLookAt(&D3DXVECTOR3(0.0f, 100.0f, -950.0f),&D3DXVECTOR3(0.0f, 0.0f, 1.0f),&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		
}

void cTestGameApp::OnCloseApplication()
{


	m_pDragon->Uninit();
	delete m_pDragon;

	m_pGround->Uninit();
	delete m_pGround;

	m_pBackWall->Uninit();
	delete m_pBackWall;
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
	cTestGameApp TestGameApp(L"ShaderSkinning",FALSE,1024,768);
	TestGameApp.Run();
	return 0;
}