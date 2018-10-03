#include "StdAfx.h"
#include "Framework.h"
#include "Framework/D3DFramework.h"
#include "Foundation/EnvironmentVariable.h"
#include "Graphics/Graphics.h"
#include "Foundation/Define.h"
#include "State.h"

using namespace Sophia;
using namespace System;
Framework::Framework(const char* szTitleName,BOOL bFullScreen,int nWidth,int nHeight)
	: Sophia::cD3DFramework(szTitleName,bFullScreen,nWidth,nHeight)
{
	m_pView = NULL;
}

void Framework::InitWindow()
{
	m_PrevFrameTime=GetTickCount();		
}

void Framework::Run()
{	
	m_CurrFrameTime=GetTickCount();		
	m_DeltaFrameTime = m_CurrFrameTime - m_PrevFrameTime;
	m_AccumFrameTime += m_DeltaFrameTime;			
	Control();
	Update(m_DeltaFrameTime);		// Update
	Render(m_DeltaFrameTime);						// Render
	m_PrevFrameTime=m_CurrFrameTime;
	
}

bool Framework::Initialize()
{
	if(!cD3DFramework::Initialize())
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
	m_pGraphics->LoadHLSL(strHLSL.c_str());

	m_pView = new State;
	AttachObject(m_pView);
	m_pView->Enter();


	return true;
}

void Framework::Finalize()
{
	if (m_pView)
	{
		m_pView->Leave();
		DettachObject(m_pView);
	}		
	SAFE_DELETE(m_pView);
	cD3DFramework::Finalize();
}
