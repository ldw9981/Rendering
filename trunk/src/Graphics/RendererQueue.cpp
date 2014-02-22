#include "StdAfx.h"
#include "RendererQueue.h"
#include "Foundation/Interface.h"

namespace Sophia
{


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

void cRendererQueue::Insert( cRendererQueue& renderQueue )
{
	m_listNode.insert(m_listNode.end(),renderQueue.m_listNode.begin(),renderQueue.m_listNode.end());
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

void cRendererQueue::Clear()
{
	m_listNode.clear();
}

}