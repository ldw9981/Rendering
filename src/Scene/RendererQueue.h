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
	
	D3DXHANDLE			m_hTechnique;// 테크닉
	std::list<IRenderer*>	m_listNode;
private:
	std::string			m_strName;
	
	
public:
	void	Insert(IRenderer* pItem);
	void	Render();
	bool	IsEmpty();
};