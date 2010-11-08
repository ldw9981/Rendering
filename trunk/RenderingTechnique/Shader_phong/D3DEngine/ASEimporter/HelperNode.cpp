#include "StdAfx.h"
#include "ASEParser.h"
#include "ASELexer.h"
#include "HelperNode.h"


cHelperNode::cHelperNode(void)
:m_strClassName("")
{
	SetNodeType(HELPER);
	m_pBoundSphere=new cSphere;
}

cHelperNode::~cHelperNode(void)
{
	delete m_pBoundSphere;
}

string& cHelperNode::GetClassName()
{
	return m_strClassName;
}

void cHelperNode::SetClassName( LPCSTR name )
{
	m_strClassName=name;
}

void cHelperNode::Init()
{

	//로컬 매트릭스 초기화	
	if (IsParentObject())
	{
		m_LocalTM = m_WorldTM;
	}
	else
	{	// m_LocalTM 을 부모기준의 상대 변환으로 설정한다. 
		m_LocalTM =	m_WorldTM * GetParentPtr()->GetWorldInverseTM();	
	}

	CalculateANMKEY(INTERPOLATE_TIME);

	InitChildren();
}


void cHelperNode::Uninit()
{

}

void cHelperNode::Render()
{
	RenderChildren();
}

void cHelperNode::Update()
{
	if (GetIsActiveAnimation())
	{
		SetAnimationTM();
		m_LocalTM = GetAnimationTM();
	}
	if (IsParentObject())	
	{		
		m_WorldTM = m_LocalTM;	
	}
	else
	{			
		m_WorldTM = m_LocalTM * GetParentPtr()->GetWorldTM();
	}	
	UpdateChildren();
}

BOOL cHelperNode::LoadObjInfo( cASEParser* pParser )
{
	char TokenString[256];
	// *BOUNDINGBOX_MIN
	if(pParser->GetToken(TokenString) != TOKENR_BOUNDINGBOX_MIN)
		return FALSE;

	// float*3
	pParser->GetVector3(&m_BoundBoxMin);

	// *BOUNDINGBOX_MAX
	if(pParser->GetToken(TokenString) != TOKENR_BOUNDINGBOX_MAX)
		return FALSE;
	// float*3
	pParser->GetVector3(&m_BoundBoxMax);

	m_pBoundSphere->Make(m_BoundBoxMin,m_BoundBoxMax);

	

	return TRUE;
}


