#pragma once
#include "Foundation/Interface.h"

class IRenderer;
class cRendererQueue:
	public IRenderer
{
public:
	cRendererQueue();
	~cRendererQueue();

private:
	list<IRenderer*> m_listNode;
public:
	void	Insert(IRenderer* pItem);
	void	Render();
};