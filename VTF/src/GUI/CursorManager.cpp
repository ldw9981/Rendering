#include "StdAfx.h"
#include "CursorManager.h"

namespace Sophia
{

CursorManager* CursorManager::m_pInstance;

CursorManager::CursorManager(void)
{
	m_pInstance = this;
	m_hWnd = NULL;
}


CursorManager::~CursorManager(void)
{

}

void CursorManager::Initialize( HWND hWnd )
{
	m_hWnd = hWnd;
}

void CursorManager::Finalize()
{

}

void CursorManager::GetCursorPos(POINT* pPoint)
{
	::GetCursorPos(pPoint);
	::ScreenToClient(m_hWnd,pPoint);
}

CursorManager* CursorManager::GetInstance() 
{
	assert(m_pInstance!=NULL); 
	return m_pInstance;
}


}