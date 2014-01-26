#pragma once
/*
	쉐이더 하나와 대응되는 렌더러를 그리는 리스트

*/
#include "Foundation/Interface.h"

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