#pragma once
#include "Foundation/interface.h"
#include "Graphics/Transformable.h"
#include "Graphics/Animation.h"
#include "Foundation/EnumString.h"

namespace Sophia
{

struct SCENEINFO; 
class cASEParser; 
class cRendererQueue;
class cCameraNode;
class cView;
class Frustum;
class Entity;
class SceneAnimation;
class EntityAnimation;
class EntityMaterial;
struct SCENENODEINFO;
typedef unsigned char SCENETYPE;


enum eSceneType 
{ 
	SCENE_ROOT,SCENE_NODE,SCENE_MESH,SCENE_SKINNEDMESH,SCENE_SKELETON
};

Begin_Enum_String( eSceneType )
{
	Enum_String(SCENE_ROOT);
	Enum_String(SCENE_NODE);
	Enum_String(SCENE_MESH);
	Enum_String(SCENE_SKINNEDMESH);
	Enum_String(SCENE_SKELETON);
}
End_Enum_String;

class cSceneNode:
	public IUnknownObject,
	public IUpdatable,	
	public cTransformable,
	public IRenderer,
	public ISerializable
{
public:

public:
	cSceneNode*				m_pParentNode;
	std::vector<SceneAnimation*>	m_vecSceneAnimation;	// Transform 애니메이션 정보
	std::vector<cSceneNode*>		m_vecChildNode;		
	std::vector<size_t>		m_partialIndex;
	D3DXMATRIX				m_referenceTM;
	D3DXMATRIX				m_nodeTM;	
protected:
	SCENETYPE 					m_type;
	std::string					m_strNodeName;			
	std::string					m_strParentName;			
	Entity*					m_pRootNode;	
	size_t					m_baseAnimationKeyIndex;
	size_t					m_basePrevAnimationKeyIndex;
	size_t					m_partialAnimationKeyIndex;
	size_t					m_partialPrevAnimationKeyIndex;
	bool					m_bIsActiveAnimation;		
	bool					m_bShow;	
public:	
	cSceneNode(void);
	virtual ~cSceneNode(void);

	const Sophia::SCENETYPE	GetType() { return m_type; }
	void				SetNodeTM(D3DXMATRIX& val) { m_nodeTM = val; }
	D3DXMATRIX&			GetNodeTM();		
	virtual void		UpdateLocalMatrix();		

	//  Object list
	// 자식 오브젝트를 추가한다.
	void				AttachChildNode(cSceneNode* pItem);
	void				DettachChildNode(cSceneNode* pItem);

	// 자신을 포함한 자식ASE오브젝트에서 ASE오브젝트를 찾는다.
	virtual cSceneNode*	FindNode(std::string& nodename);

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
	bool				GetIsActiveAnimation() const { return m_bIsActiveAnimation; }
	void				SetIsActiveAnimation(bool val) { m_bIsActiveAnimation = val; }

	bool GetShow() const { return m_bShow; }
	void SetShow(bool val) { m_bShow = val; }
	
	void				FreeChildren();
	
	cSceneNode&	operator=(const cSceneNode& other);


	virtual void		Render(DWORD elapseTime);
	virtual void		Update(DWORD elapseTime);

	/*
		빌드란 씬노드구성후 작동하기전에 노드의 후처리 작업으로 완성시킴을 의미한다.
	*/
	virtual	void		BuildComposite(Entity* pEntity);

	// ISerialize
	virtual void		SerializeIn(std::ifstream& stream);
	virtual void		SerializeOut(std::ofstream& stream);

	virtual void		GatherRender(Entity* pEntity,bool bTraversal);
	virtual void		Release();	

	cSceneNode*		CreateNode(SCENETYPE type);

	virtual void PushAnimation(EntityAnimation* pEntityAnimation);
	virtual void PopAnimation();
	virtual void EraseAnimation(int index);
	
	void WorkRecursive(void(*Func)(cSceneNode*));
	void AddPatialIndex();
	void DelPartialIndex(size_t index);
	SceneAnimation* GetSceneAnimation(size_t index);
	int CountSceneAnimation();
};

}