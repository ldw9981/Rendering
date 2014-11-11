#pragma once
#include "Scene/scenenode.h"
#include "Math/Sphere.h"
#include "Graphics/RendererQueue.h"
#include "Graphics/Animation.h"
#include "Graphics.h"

namespace Sophia
{

class EntityAnimation;
class EntityMaterial;
class Skeleton;
class Entity :
	public cSceneNode
{
public:
	Entity(void);
	virtual ~Entity(void);

public:
	cRendererQueue			m_renderQueueNormal;				
	cRendererQueue			m_renderQueueSkinned;				
	cRendererQueue			m_renderQueueNormalAlphaBlend;	
	cRendererQueue			m_renderQueueSkinnedAlphaBlend;	

	cRendererQueue			m_renderQueueTerrain;


	std::list<Entity*>::iterator m_itEntityList;
	std::vector<EntityAnimation*>	m_vecAnimation;		
	std::map<std::string,cSceneNode*>	m_mapBones;
	EntityMaterial*			m_pEntityMaterial;


	bool				m_bShowBone;
	
	ENTITY_ANIMATION_DESCRIPTION m_baseAnimationDesc;		
	ENTITY_ANIMATION_DESCRIPTION m_basePrevAnimationDesc;	//�ٸ� �ִ��̼� ��ȯ�Ҷ� fadeIn �ɼ� ����Ѵٸ� ���� �ִϸ��̼�Ű�� ��� ������� �ʿ��ϴ�.
	std::list<ENTITY_ANIMATION_DESCRIPTION> m_listPartial;
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
	virtual void UpdateLocalMatrix();
	
	

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
	void PlayBaseAnimation(int index,bool loop,DWORD skipStartTime=0,DWORD earlyEndTime=0,DWORD fadeInTime=0,DWORD playTime=0);
	void StopBaseAnimation();
	void PlayPartialAnimation(int index,bool loop,DWORD skipStartTime=0,DWORD earlyEndTime=0);
	void StopPartialAnimation(int index);

	void InsertBone(cSceneNode* pBone);
	const std::vector<EntityAnimation*> GetVecAnimation() { return m_vecAnimation; }

	void UpdateAnimationDescription(DWORD elapseTime,ENTITY_ANIMATION_DESCRIPTION& desc);
	bool UpdatePartialDescription(DWORD elapseTime,std::list<ENTITY_ANIMATION_DESCRIPTION>::iterator& it);
	void SetShowSkeleton(bool bShow);

	void ClearRenderQueue();
	virtual cSceneNode*	FindNode(std::string& nodename);
	void ResetRenderQueue();

	void ChangeInstanceEnable(bool enable);
	static void WorkEnableInstancing(cSceneNode* pNode);
	static void WorkDisableInstancing(cSceneNode* pNode);
};


}