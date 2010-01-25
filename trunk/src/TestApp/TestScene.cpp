#include "TestScene.h"
#include "../Scene/SceneMng.h"
#include "../ASEParser/ASEParser.h"
#include "../Framework/App/App.h"
#include "../GUI/GUIButton.h"

#include "TestGameApp.h"
#include "Ground.h"
#include "ObjDragon.h"
#include "Ground.h"

#include "TestScene.h"
#include "MenuScene.h"
#include "GlobalScene.h"




TestScene::TestScene(void)
:m_pDragon(NULL)
{
	SetViewPortInfo(0,0,1024,768);

	m_pDragon = new cObjDragon;
	cASEParser parser;
	parser.Load(L"../../export/Dragon2.ase",m_pDragon);
	m_pDragon->Init();
	AttachObject(m_pDragon);
}

TestScene::~TestScene(void)
{
	DettachObject(m_pDragon);
	SAFE_DELETE(m_pDragon);



}


void TestScene::Open(void* arg)
{
	m_Camera.SetActive();
	m_Camera.SetPerspective(D3DXToRadian(45),1.0f,10000.0f,
		(float)Application::App::Instance()->GetRequestRectWidth(),
		(float)Application::App::Instance()->GetRequestRectHeight());
	m_Camera.SetLookAt(&D3DXVECTOR3(0.0f, 100.0f, -950.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		
}

void TestScene::Close()
{
	
}

void TestScene::Control()
{	
	if (m_SharedWinInput.IsTurnDn(VK_SPACE))
	{
		Application::TestGameApp* p=(Application::TestGameApp*)Application::App::Instance();
			
		p->m_pGlobalScene->SetHide(!p->m_pGlobalScene->GetHide());
		
	}
	Scene::Control();
}

void TestScene::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{

}

void TestScene::Update( DWORD elapseTime )
{
	Scene::Update(elapseTime);
	m_Camera.Update(elapseTime);
}

void TestScene::ProcessRender()
{
	CameraNode::GetActiveCamera()->Render();
	Scene::ProcessRender();
}