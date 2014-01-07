#pragma once
/*
	���̴� �ϳ��� �����Ǵ� �������� �׸��� ����Ʈ

*/
#include "Foundation/Interface.h"

class IRenderer;
class cRendererQueue:
	public IRenderer
{
public:
	cRendererQueue();
	~cRendererQueue();
	
	std::list<IRenderer*>	m_listNode;
private:
public:
	void	Insert(IRenderer* pItem);
	void	Render();
	bool	IsEmpty();
};

class RendererQueue:
	public IRenderer
{
public:
	RendererQueue();
	~RendererQueue();

	std::list<IRenderer*>		m_listNode;
private:
public:
	void	Insert(IRenderer* pItem);
	void	Render();
	bool	IsEmpty();
};