#include "StdAfx.h"
#include "TestStateA.h"
#include "ASEParser/ASEParser.h"
#include "TestGameApp.h"
#include "ObjTank.h"
#include "Framework/EnvironmentVariable.h"

TestStateA::TestStateA(void)
{
	m_Camera.SetPerspective(D3DXToRadian(45),1.0f,10000.0f,
		(float)g_pApp->GetRequestRectWidth(),(float)g_pApp->GetRequestRectHeight());
	m_Camera.SetLookAt(&D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		

	m_Camera.SetLocalPos(D3DXVECTOR3(0.0f,0.0f,-2000.0f));	
}


TestStateA::~TestStateA(void)
{
}

void TestStateA::Enter()
{
	SetViewPortInfo(0,0,g_pApp->GetRequestRectWidth(),g_pApp->GetRequestRectHeight());
	m_Camera.SetActive();

	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
	m_pTank = new cObjTank;
	cASEParser parser;
	//parser.Load(std::string(strDataPath+"03ik-joe.ASE").c_str(),m_pTank);
	parser.Load(std::string(strDataPath+"TigerTank.ase").c_str(),m_pTank);

	m_pTank->BuildComposite();
	m_pTank->Init();
	AttachObject(m_pTank);
}

void TestStateA::Leave()
{
	DettachObject(m_pTank);
	SAFE_DELETE(m_pTank);	
}

void TestStateA::Control()
{

}

void TestStateA::Update( DWORD elapseTime )
{
	cView::Update(elapseTime);
	m_Camera.Update(elapseTime);
}

void TestStateA::ProcessRender()
{
	m_Camera.Render();
	cView::ProcessRender();
}








