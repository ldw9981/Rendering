#pragma once
/*
	���̴� �ϳ��� �����Ǵ� �������� �׸��� ����Ʈ

*/
#include "Foundation/Interface.h"
namespace Sophia
{

class IRenderer;
class cRendererQueue:
	public IRenderer
{
public:
	cRendererQueue();
	~cRendererQueue();
	
	std::list<std::pair<IRenderer*,unsigned char>>	m_listNode;
private:
public:
	void	Insert(IRenderer* pItem,unsigned char index);
	void	Insert(cRendererQueue& renderQueue);
	void	Render();
	bool	IsEmpty();
	void	Clear();
};

class RendererQueue:
	public IRenderer
{
public:
	RendererQueue();
	~RendererQueue();

	std::list<std::pair<IRenderer*,unsigned char>>		m_listNode;
private:
public:
	void	Insert(IRenderer* pItem,unsigned char index);
	void	Render();
	bool	IsEmpty();
};
}