#pragma once
#include "scenenode.h"
#include "Math/Sphere.h"
#include "Scene/RendererQueue.h"

class Entity :
	public cSceneNode
{
public:
	Entity(void);
	virtual ~Entity(void);

public:
	cRendererQueue			m_renderQueueNormal[16];
	cRendererQueue			m_renderQueueBlend[16];
	cRendererQueue			m_renderQueueTerrain;
	cRendererQueue			m_renderQueueGUI;

	cRendererQueue			m_renderQueueNormalShadow;
	cRendererQueue			m_renderQueueBlendShadow;
	std::list<Entity*>::iterator m_itEntityList;

protected:
	cSphere				m_BoundingSphere;		// �⺻ �� (�ѹ��������ϸ� �������� ��ġ�� ����)
	
public:
	void				SetBoundingSphere(cSphere& Sphere);
	cSphere&			GetBoundingSphere()  { return m_BoundingSphere; }

	virtual bool		Cull(Frustum* pFrustum ,float loose);
	void				RenderBound();
	virtual void		Update(DWORD elapseTime);
	virtual	void		Build();

	bool	Save(const char* fileName);
	bool	Load(const char* fileName);


	virtual void SerializeIn(std::ifstream& stream);
	virtual void SerializeOut(std::ofstream& stream);
};

