#include "StdAfx.h"
#include "State.h"
#include "Framework/D3DFramework.h"
#include "Foundation/EnvironmentVariable.h"
#include "Graphics/Graphics.h"
#include "Foundation/StringUtil.h"
#include "Input/Input.h"


#define PI           3.14159265f
#define FOV          (PI/4.0f)	

using namespace Sophia;

State::State(void)
{
	m_bControlCamera=FALSE;

	m_pZTerrain=NULL;

	m_pTank=NULL;
	m_pAirPlaneBake=NULL;
	m_pDragon=NULL;

	for (int i=0;i<10;i++)
	{
		m_pHouse[i] = NULL;
	}
	m_graphicWorld.SetViewPortInfo(0,0,1024,768);
	m_pModel = NULL;
	m_bModifiedAnimation = false;
	m_bModifiedScene = false;
	m_bModifiedMaterial = false;	
	m_scrollSize = 100.0f;
	m_nStressCount = 100;
}

State::~State( void )
{

}
void State::Enter()
{
	cView::Enter();
	m_graphicWorld.m_camera.SetPerspective(FOV,1.0f,15000.0f,
		(float)g_pApp->GetRequestRectWidth(),(float)g_pApp->GetRequestRectHeight());
	m_graphicWorld.m_camera.SetLookAt(&D3DXVECTOR3(0.0f, 0.0f, -1500.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	

	m_graphicWorld.SetWorldLightDirection(D3DXVECTOR3(0.0f, -1.0f, 1.0f));
	m_graphicWorld.SetEnableShadow(true);

	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");

	/*
	m_pZTerrain = m_graphicWorld.CreateTerrain(&D3DXVECTOR3(20.0f,0.5f,20.0f),
		std::string(strDataPath+"map129.bmp").c_str(),
		std::string(strDataPath+"ground.bmp").c_str());
	*/
	/*
	m_pAirPlaneBake = m_graphicWorld.CreateEntity();
	m_pAirPlaneBake->LoadASE(std::string(strDataPath+"AirPlaneBake.ase").c_str());
	m_pAirPlaneBake->Build();
	//m_pAirPlaneBake->Init();
	m_pAirPlaneBake->SetVelocityRotation(D3DXVECTOR3(0.0f,-45,0.0f));
	m_pAirPlaneBake->SetLocalPos(D3DXVECTOR3(-300.0f,100.0f,-100.0f));	
	*/
}

void State::Leave()
{
	/*
	m_graphicWorld.DeleteTerrain(m_pZTerrain);
	//DettachObject(m_pZTerrain);
	//SAFE_DELETE(m_pZTerrain);
	*/
	//m_graphicWorld.DeleteEntity(m_pAirPlaneBake);
	
	if (m_pModel)
	{
		m_graphicWorld.DeleteEntity(m_pModel);
	}
	ClearStressEntity();
	cView::Leave();
}


void State::Update( DWORD elapseTime )
{
	m_helper.Update(elapseTime);
	cView::Update(elapseTime);		
}

void State::ProcessRender()
{
	cView::ProcessRender();	
}

void State::Control()
{
	cView::Control();		
	

	if (g_pInput->IsTurnDn(DIK_F12))
	{
		
		m_graphicWorld.m_bDebugBound = !m_graphicWorld.m_bDebugBound;
	}	
	
	if (m_pModel)
	{
		if (g_pInput->GetMouseState().lX != 0)
		{
			if(g_pInput->Mouse_IsCurrDn(0))
			{
				int* delta = (int*)&g_pInput->GetMouseState().lX;


				D3DXMATRIX& localTM = m_helper.GetLocalTM();
				D3DXMATRIX rot;
				D3DXMatrixRotationY(&rot,D3DXToRadian( *(delta) * -0.2f ));
				localTM = rot * localTM;
			}
		}
		else if (g_pInput->GetMouseState().lY != 0)
		{
			if(g_pInput->Mouse_IsCurrDn(0))
			{
				int* delta = (int*)&g_pInput->GetMouseState().lY;

				D3DXMATRIX& localTM = m_helper.GetLocalTM();
				D3DXMATRIX rot;
				D3DXMatrixRotationX(&rot,D3DXToRadian( *(delta) * -0.2f ));
				localTM = rot * localTM;
			}
		}
	}

	if (g_pInput->GetMouseState().lZ != 0)
	{

			int iz = *(int*)&g_pInput->GetMouseState().lZ;
			iz /= abs(iz);
			D3DXVECTOR3 temp;
			m_graphicWorld.m_camera.GetLocalPos(temp);
			temp.z += (float)(iz) * m_scrollSize;		
			m_graphicWorld.m_camera.SetLocalPos(temp);
	}
	
}

void State::OpenASE( const char* path )
{
	if (m_pModel)
	{
		m_graphicWorld.DeleteEntity(m_pModel);
		m_pModel = NULL;
	}
	m_pModel = m_graphicWorld.CreateEntity();
	m_pModel->LoadASE(path);
	m_pModel->Build();	
	m_pModel->SetParentNode(&m_helper);
	D3DXMatrixIdentity(&m_helper.GetLocalTM());

	m_bModifiedAnimation = true;
	m_bModifiedScene = true;
	m_bModifiedMaterial = true;
}

void State::SaveAsset()
{
	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
	m_pModel->SaveScene(std::string(strDataPath+m_pModel->GetNodeName()+".scene").c_str());
	m_pModel->SaveAnimationSet(std::string(strDataPath+m_pModel->GetNodeName()+".aniset").c_str());		
	m_pModel->SaveMaterial(std::string(strDataPath+m_pModel->GetNodeName()+".material").c_str(),0);

	m_bModifiedAnimation = false;
	m_bModifiedScene = false;
	m_bModifiedMaterial = false;
}



void State::OpenAsset( const char* path )
{
	if (m_pModel)
	{
		m_graphicWorld.DeleteEntity(m_pModel);
		m_pModel = NULL;
	}
	m_pModel = m_graphicWorld.CreateEntity();

	std::string dir,name;
	StringUtil::SplitPath(std::string(path),NULL,&dir,&name,NULL);
	m_pModel->LoadScene(std::string(dir+name+".scene").c_str());
	m_pModel->LoadAnimationSet(std::string(dir+name+".aniset").c_str());
	m_pModel->LoadMaterial(std::string(dir+name+".material").c_str());
	m_pModel->Build();
	m_pModel->SetParentNode(&m_helper);
	D3DXMatrixIdentity(&m_helper.GetLocalTM());

	//ClearStressEntity();
	//OpenStressEntity(path);
}

Sophia::Entity* State::GetEntity()
{
	return m_pModel;
}

void State::ClearStressEntity()
{
	for (auto it = m_vecStress.begin();it!=m_vecStress.end();++it)
	{
		m_graphicWorld.DeleteEntity(*it);
	}
	m_vecStress.clear();
}

void State::OpenStressEntity( const char* asset )
{
	Sophia::Entity* pEntity=NULL;
	
	for (int i=0;i<m_nStressCount;i++)
	{
		pEntity = m_graphicWorld.CreateEntity();
		m_vecStress.push_back(pEntity);

		std::string dir,name;
		StringUtil::SplitPath(std::string(asset),NULL,&dir,&name,NULL);
		pEntity->LoadScene(std::string(dir+name+".scene").c_str());
		pEntity->LoadAnimationSet(std::string(dir+name+".aniset").c_str());
		pEntity->LoadMaterial(std::string(dir+name+".material").c_str());
		pEntity->Build();

		D3DXVECTOR3 pos;
		pos.x = (rand()%100 - 50)*100.0f;
		pos.y = (rand()%100 - 50)*1.0f;
		pos.z = (rand()%100 - 50)*100.0f;
		pEntity->SetLocalPos(pos);

	}
}



