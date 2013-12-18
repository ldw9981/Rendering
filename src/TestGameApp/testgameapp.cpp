#include "StdAfx.h"
#include "TestGameApp.h"

#include "Resource/ResourceMng.h"
#include "D3D9Server/RscTexture.h"

#include "Scene/ViewMng.h"
#include "TestView.h"
#include "MenuView.h"
#include "GlobalView.h"
#include "Framework/D3DFramework.h"
#include "D3D9Server/Server.h"
#include "D3D9Server/MaterialEx.h"

using namespace D3D9;

TestGameApp::TestGameApp( const char* szTitleName,BOOL bFullScreen,int nWidth,int nHeight)
:cD3DFramework(szTitleName,bFullScreen,nWidth,nHeight)
{
	m_pMenuScene=NULL;
	m_pTestScene=NULL;
	m_pGlobalScene=NULL;
	
}

TestGameApp::~TestGameApp(void)
{
}

bool TestGameApp::Open()
{
	if(!cD3DFramework::Open())
		return false;
	
	std::string Path;
	Path = EnvironmentVariable::GetInstance().GetString("CurrPath");

	std::string::size_type index=Path.length();
	for (int i=0;i<3;i++)
	{
		index=Path.rfind("\\",index-1,1);
	}	
	Path  = Path.substr ( 0 ,index+1);		
	Path  += "Data\\";

	EnvironmentVariable::GetInstance().SetString("DataPath",Path.c_str());
	

	std::string strHLSL=Path;
	strHLSL+= "hlsl.fx";
	m_pD3D9Server->LoadHLSL(strHLSL.c_str());

	std::bitset<Material::MAX> indexRenderQueue;

	indexRenderQueue.set(Material::DIFFUSE);
	g_pD3DFramework->m_listRenderQueue[indexRenderQueue.to_ulong()].m_hTechnique = m_pD3D9Server->m_hTPhongDiffuse;

	indexRenderQueue.set(Material::NORMAL);
	g_pD3DFramework->m_listRenderQueue[indexRenderQueue.to_ulong()].m_hTechnique = m_pD3D9Server->m_hTPhongDiffuseBump;

	indexRenderQueue.reset(Material::NORMAL);
	indexRenderQueue.set(Material::LIGHT);
	g_pD3DFramework->m_listRenderQueue[indexRenderQueue.to_ulong()].m_hTechnique = m_pD3D9Server->m_hTPhongDiffuseLight;

	for (int i=0;i<16;i++)
	{	
		if (g_pD3DFramework->m_listRenderQueue[i].m_hTechnique == NULL )	
			g_pD3DFramework->m_listRenderQueue[i].m_hTechnique = m_pD3D9Server->m_hTPhongDiffuse;
	}

	for (int i=0;i<16;i++)
		g_pD3DFramework->m_listRenderQueueSkinned[i].m_hTechnique = m_pD3D9Server->m_hTSkinningPhongDiffuse;

//	m_pMenuScene = new cMenuView;
	m_pTestScene = new cTestView;
//	m_pGlobalScene = new cGlobalView;
	GetSceneMng()->ChangeTopScene((cView*)m_pTestScene);
//	AttachObject(m_pGlobalScene);
	return true;
}

void TestGameApp::Close()
{
//	delete  m_pGlobalScene;	
	delete	m_pTestScene;
//	delete	m_pMenuScene;

	cD3DFramework::Close();
}


BOOL TestGameApp::OnWM_Keyboard( MSG& msg )
{
	return FALSE;
}


BOOL TestGameApp::OnWM_Mouse( MSG& msg )
{
	return FALSE;
}

BOOL TestGameApp::OnWM_General( MSG& msg )
{
	switch(msg.message)
	{
	case WM_ACTIVATEAPP:
		if (!msg.wParam)
		{
			//MessageBox(NULL,"the window is being deactivated.","알림",MB_OK);
		}
		else
		{
			//MessageBox(NULL,"the window is being activated.","알림",MB_OK);
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
	//HeapValidator::SetDbgFlag();
	//HeapValidator::SetBreakAlloc(53612);	
	
	char buffer[256];
	::GetCurrentDirectory(256,buffer);

	TestGameApp TestGameApp("TestGameApp",FALSE,1024,768);
	if(TestGameApp.Open())
	{
		TestGameApp.Run();
	}
	TestGameApp.Close();
	
	//HeapValidator::CheckMemory();
	return 0;
}