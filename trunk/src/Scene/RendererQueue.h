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
	
	D3DXHANDLE			m_hTechnique;// ��ũ��
	std::list<IRenderer*>	m_listNode;
private:
	std::string			m_strName;
	
	
public:
	void	Insert(IRenderer* pItem);
	void	Render();
	bool	IsEmpty();
};