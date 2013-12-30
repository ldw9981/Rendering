#include "StdAfx.h"
#include "RendererQueue.h"
#include "Foundation/Interface.h"



cRendererQueue::cRendererQueue()
{

}

cRendererQueue::~cRendererQueue()
{

}


void cRendererQueue::Insert( IRenderer* pItem )
{
	m_listNode.push_back(pItem);
}

void cRendererQueue::Render()
{
 	std::list<IRenderer*>::iterator it=m_listNode.begin();
 	for ( ; it!=m_listNode.end(); ++it )
 	{
 		(*it)->Render();
 	}
	m_listNode.clear();
}

bool cRendererQueue::IsEmpty()
{
	return m_listNode.empty();
}

