#include "StdAfx.h"
#include "ASEParser.h"
#include "ASELexer.h"
#include "SceneNode.h"
#include "ShapeNode.h"

cShapeNode::cShapeNode(void)
{
	SetNodeType(SHAPE);
}

cShapeNode::~cShapeNode(void)
{
}



BOOL cShapeNode::LoadObjInfo( cASEParser* pParser )
{
	char tokenString[256];
	pParser->GetToken(tokenString);
	m_NUMLINE = pParser->GetInt();

	for (int i=0;i<m_NUMLINE;i++)
	{
		pParser->GetToken(tokenString);
		pParser->GetToken(tokenString);
		pParser->SkipBlock();
	}
	return TRUE;
}

void cShapeNode::Init()
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

void cShapeNode::Uninit()
{

}

void cShapeNode::Update()
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

void cShapeNode::Render()
{
	RenderChildren();
}