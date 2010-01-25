#pragma once
#include "../Framework/Interface.h"

class IRenderer;
class cRendererQueue:
	public Framework::IRenderer
{
public:
	cRendererQueue();
	~cRendererQueue();

private:
	list<Framework::IRenderer*> m_listNode;
public:
	void	Insert(Framework::IRenderer* pItem);
	void	Render();
};