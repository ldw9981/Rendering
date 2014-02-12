#include "StdAfx.h"
#include "GUIBase.h"

cGUIBase::cGUIBase(void)
{
	m_pWorld = NULL;
}

cGUIBase::~cGUIBase(void)
{
}

void cGUIBase::ProcessRender()
{
	Render();
}