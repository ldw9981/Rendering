#include "StdAfx.h"
#include "ASEParser.h"
#include "ASELexer.h"
#include "HelperNode.h"


cHelperNode::cHelperNode(void)
:m_strClassName(L"")
{
	SetNodeType(HELPER);
	m_pBoundSphere=new cSphere;
}

cHelperNode::~cHelperNode(void)
{
	delete m_pBoundSphere;
}

wstring& cHelperNode::GetClassName()
{
	return m_strClassName;
}

void cHelperNode::SetClassName( LPCWSTR name )
{
	m_strClassName=name;
}

void cHelperNode::Init()
{
	InitChildren();
}


void cHelperNode::Uninit()
{
	UninitChildren();
}

void cHelperNode::Render()
{
	RenderChildren();
}

void cHelperNode::Update(DWORD elapseTime)
{
	if (GetIsActiveAnimation())
	{
		UpdateTransformAnm(elapseTime);
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
	UpdateChildren(elapseTime);
}

