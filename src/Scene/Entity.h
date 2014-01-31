#pragma once
#include "scenenode.h"
#include "Math/Sphere.h"
#include "Scene/RendererQueue.h"

class EntityAnimation;
class EntityMaterial;

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
	std::vector<EntityAnimation*>	m_vecAnimation;		
	std::vector<EntityMaterial*>	m_vecMaterial;		
	int					m_indexMaterial;
	int					m_animationIndex;
	DWORD				m_animationTime;
	bool				m_animationLoop;
protected:
	cSphere				m_BoundingSphere;		// 기본 구 (한번만설정하며 매프레임 위치만 갱신)
	
public:
	void				SetBoundingSphere(cSphere& Sphere);
	cSphere&			GetBoundingSphere()  { return m_BoundingSphere; }

	virtual bool		Cull(Frustum* pFrustum ,float loose);
	void				RenderBound();
	virtual void		Update(DWORD elapseTime);
	virtual	void		Build();

	virtual void SerializeIn(std::ifstream& stream);
	virtual void SerializeOut(std::ofstream& stream);

	virtual void PushAnimation(EntityAnimation* pEntityAnimation);
	virtual void PopAnimation();

	virtual void PushMaterial(EntityMaterial* pEntityMaterial);
	virtual void PopMaterial();


	bool SaveScene(const char* fileName);
	bool LoadScene(const char* fileName);
		 
	bool SaveAnimation(const char* fileName,int index);
	bool LoadAnimation(const char* fileName);
		 
	bool SaveMaterial(const char* fileName,int index);
	bool LoadMaterial(const char* fileName);

	void CutAndPushEntityAnimation( int index,DWORD timeStart,DWORD timeEnd,char* suffix );
	void PlayAnimation(int index,bool loop);
};

