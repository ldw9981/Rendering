#pragma once
#include "../Common/Interface.h"

class cIRenderer;
class cRendererQueue:
	public cIRenderer
{
public:
	cRendererQueue();
	~cRendererQueue();

private:
	list<cIRenderer*> m_listNode;
public:
	void	Insert(cIRenderer* pItem);
	void	Render();
};