#pragma once
#include "Foundation/interface.h"
#include "Scene/Transformable.h"


struct SCENEINFO; 
class cASEParser; 
class cRscTransformAnm;
class cRendererQueue;
class cCameraNode;
class cView;
class Frustum;
class Entity;
class SceneAnimation;
class EntityAnimation;
class EntityMaterial;
class SceneMaterial;

struct SCENENODEINFO;
typedef unsigned char SCENETYPE;
class cSceneNode:
	public IUnknownObject,
	public IUpdatable,	
	public cTransformable,
	public IRenderer,
	public ISerializable
{
public:
	cSceneNode(void);
	virtual ~cSceneNode(void);
	
	enum TYPE { TYPE_SCENE,TYPE_MESH,TYPE_SKINNEDMESH};
protected:
	SCENETYPE 					m_type;
	std::string					m_strNodeName;			
	std::string					m_strParentName;		
	std::list<cSceneNode*>		m_listChildNode;		
	cSceneNode*				m_pParentNode;
	Entity*					m_pRootNode;
	bool					m_bIsBone;

	
	// Transform �ִϸ��̼� ����
	std::vector<SceneAnimation*>	m_vecSceneAnimation;
	

	BOOL					m_bIsActiveAnimation;
	D3DXMATRIX				m_AnimationTM;			
	bool					m_bRender;
	DWORD					m_animationTime;
	
	std::list<cSceneNode*>::iterator m_ParentListIt;
public:	
	D3DXMATRIX				m_nodeTM;

	void				SetNodeTM(D3DXMATRIX& val) { m_nodeTM = val; }
	D3DXMATRIX&			GetNodeTM();
		
	D3DXMATRIX*			UpdateSceneAnimation(DWORD& animationTime,DWORD elapseTime);
	D3DXMATRIX&			GetAnimationTM();
			

	//  Object list
	// �ڽ� ������Ʈ�� �߰��Ѵ�.
	void				AttachChildNode(cSceneNode* pItem);
	void				DettachChildNode(cSceneNode* pItem);

	// �ڽ��� ������ �ڽ�ASE������Ʈ���� ASE������Ʈ�� ã�´�.
	cSceneNode*			FindNode(std::string& nodename);

	//Get/Set
	void				SetNodeName( const char* nodename );
	std::string&				GetNodeName();

	void				SetParentName( const char* nodename );
	std::string&				GetParentName();
	BOOL				IsExistParentName();		
	BOOL				IsRootNode();
	
	cSceneNode*			GetParentNode() const { return m_pParentNode; }
	void				SetParentNode(cSceneNode* val) { m_pParentNode = val; }


	Entity*				GetRootNode() const { return m_pRootNode; }
	void				SetRootNode(Entity* val) { m_pRootNode = val; }
	bool				GetIsBone() const { return m_bIsBone; }
	void				SetIsBone(bool val) { m_bIsBone = val; }	


	BOOL				GetIsActiveAnimation() const { return m_bIsActiveAnimation; }
	void				SetIsActiveAnimation(BOOL val) { m_bIsActiveAnimation = val; }

	bool GetRender() const { return m_bRender; }
	void SetRender(bool val) { m_bRender = val; }
	void				UpdateChildren(DWORD elapseTime);
	void				RenderChildren();
	void				FreeChildren();
	
	cSceneNode&	operator=(const cSceneNode& other);


	virtual void		Render();
	virtual void		RenderShadow();
	virtual void		Update(DWORD elapseTime);

	/*
		����� ����屸���� �۵��ϱ����� ����� ��ó�� �۾����� �ϼ���Ŵ�� �ǹ��Ѵ�.
	*/
	virtual	void		BuildComposite(Entity* pEntity);

	// ISerialize
	virtual void		SerializeIn(std::ifstream& stream);
	virtual void		SerializeOut(std::ofstream& stream);

	virtual void		QueueRenderer(Entity* pEntity,bool bTraversal);
	virtual void		QueueRendererShadow(Entity* pEntity,bool bTraversal);
	virtual void		Release();	

	cSceneNode*		CreateNode(SCENETYPE type);

	void		SerializeInAnm(std::ifstream& stream);
	void		SerializeOutAnm(std::ofstream& stream);

	virtual void PushAnimation(EntityAnimation* pEntityAnimation);
	virtual void PopAnimation();
	virtual void PushMaterial(EntityMaterial* pEntityMaterial);
	virtual void PopMaterial();
};

