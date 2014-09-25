#pragma once
/*
	쉐이더 하나와 대응되는 렌더러를 그리는 리스트

*/
#include "Foundation/Interface.h"
namespace Sophia
{

class IRenderer;
class Material;
class MultiSub;
class cCameraNode;
class cRendererQueue:
	public IRenderer
{
public:
	cRendererQueue();
	~cRendererQueue();

	struct Specific 
	{
		MultiSub* pMultiSub;
		Material* pMaterial;
		float distancesq;

	};


	std::list<std::pair<IRenderer*,Specific>>	m_listNode;
private:
public:
//	void	Insert(IRenderer* pItem,unsigned char index);
	void	Insert(IRenderer* pItem,MultiSub* pMultiSub,Material* pMaterial);
	void	Insert(cRendererQueue& renderQueue);
	void	Render();
	void	RenderAlphaTest();
	void	RenderAlphaBlendAndTest(cCameraNode* pCamera);
	bool	IsEmpty();
	void	Clear();
	void	Sort(cCameraNode* pCamera);
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