#include "StdAfx.h"
#include "RendererQueue.h"
#include "Foundation/Interface.h"



cRendererQueue::cRendererQueue()
{

}

cRendererQueue::~cRendererQueue()
{
	
}


void cRendererQueue::Insert( IRenderer* pItem ,unsigned char index)
{
	m_listNode.push_back(std::make_pair(pItem,index));
}

void cRendererQueue::Render()
{
 	auto it=m_listNode.begin();
 	for ( ; it!=m_listNode.end(); ++it )
 	{		
 		(*it).first->Render( (*it).second);
 	}
}

bool cRendererQueue::IsEmpty()
{
	return m_listNode.empty();
}

