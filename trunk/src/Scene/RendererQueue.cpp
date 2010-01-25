
#include "RendererQueue.h"
#include "../Framework/Interface.h"



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
 	list<IRenderer*>::iterator it=m_listNode.begin();
 	for ( ; it!=m_listNode.end(); ++it )
 	{
 		(*it)->Render();
 	}
	m_listNode.clear();
}