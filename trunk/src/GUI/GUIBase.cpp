#include "StdAfx.h"
#include "GUIBase.h"
namespace Sophia
{

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
}