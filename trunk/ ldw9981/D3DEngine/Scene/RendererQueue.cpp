#include "StdAfx.h"
#include "RendererQueue.h"
#include "../Common/Interface.h"



cRendererQueue::cRendererQueue()
{

}

cRendererQueue::~cRendererQueue()
{

}


void cRendererQueue::Insert( cIRenderer* pItem )
{
	m_listNode.push_back(pItem);
}

void cRendererQueue::Render()
{
 	list<cIRenderer*>::iterator it=m_listNode.begin();
 	for ( ; it!=m_listNode.end(); ++it )
 	{
 		(*it)->Render();
 	}
	m_listNode.clear();
}