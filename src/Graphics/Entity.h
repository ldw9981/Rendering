#pragma once
#include "Scene/scenenode.h"
#include "Math/Sphere.h"
#include "Graphics/RendererQueue.h"
#include "Graphics/Animation.h"

namespace Sophia
{

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
	std::map<std::string,cSceneNode*>	m_mapBones;
	int					m_indexMaterial;
	
	ENTITY_ANIMATION_DESCRIPTION m_animationDesc;		
	ENTITY_ANIMATION_DESCRIPTION m_animationDescPrev;	//�ٸ� �ִ��̼� ��ȯ�Ҷ� fadeIn �ɼ� ����Ѵٸ� ���� �ִϸ��̼�Ű�� ��� ������� �ʿ��ϴ�.
protected:
	cSphere				m_BoundingSphere;		// �⺻ �� (�ѹ��������ϸ� �������� ��ġ�� ����)
	
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
	virtual void EraseAnimation(int index);

	virtual void PushMaterial(EntityMaterial* pEntityMaterial);
	virtual void PopMaterial();

	

	bool LoadASE(const char* fileName);
	bool SaveScene(const char* fileName);
	bool LoadScene(const char* fileName);
		 
	bool SaveAnimation(const char* fileName,int index);
	bool LoadAnimation(const char* fileName);
	bool SaveAnimationSet(const char* fileName);	 
	bool LoadAnimationSet(const char* fileName);	

	bool SaveMaterial(const char* fileName,int index);
	bool LoadMaterial(const char* fileName);

	void CutAndPushEntityAnimation( int index,DWORD timeStart,DWORD timeEnd,const char* suffix );
	void PlayAnimation(int index,bool loop,DWORD skipStartTime=0,DWORD earlyEndTime=0,DWORD fadeInTime=0);
	void StopAnimation();

	void InsertBone(const char* name,cSceneNode* pBone);
	const std::vector<EntityAnimation*> GetVecAnimation() { return m_vecAnimation; }

	void UpdateAnimationDescription(DWORD elapseTime,ENTITY_ANIMATION_DESCRIPTION& desc);
};


}