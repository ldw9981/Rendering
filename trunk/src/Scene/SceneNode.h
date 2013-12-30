#pragma once
#include "Foundation/interface.h"
#include "Scene/Transformable.h"
#include "Math/Sphere.h"

struct SCENEINFO; 
class cASEParser; 
class cRscTransformAnm;
class cRendererQueue;
class cSphere;
class cCameraNode;
class cView;
struct SCENENODEINFO;

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
	
protected:
	std::string					m_strNodeName;			
	std::string					m_strParentName;		
	std::list<cSceneNode*>		m_listChildNode;		
	cSceneNode*				m_pParentNode;
	cSceneNode*				m_pRootNode;

	// 아래두개는 버텍스 버퍼가 있을때만 만들어진다.
	cSphere*				m_pCullingSphere;		// 자식의 컬링구 를 모두 포함하는구(위치,볼륨을 매프레임 갱신됨) 
	cSphere					m_BoundingSphere;		// 기본 구 (한번만설정하며 매프레임 위치만 갱신)
	
	// Transform 애니메이션 정보
	cRscTransformAnm*		m_pRscTransformAnm;	

	BOOL					m_bIsActiveAnimation;
	D3DXMATRIX				m_AnimationTM;			
	bool					m_bRender;

	std::list<cSceneNode*>::iterator m_ParentListIt;

public:	
	void				SetNodeInfo(SCENENODEINFO& stInfo);
	cRscTransformAnm*	GetRscTransformAnm() const { return m_pRscTransformAnm; }
	void				SetRscTransformAnm(cRscTransformAnm* val);
		
	D3DXMATRIX*			UpdateTransformAnm(DWORD elapseTime);
	D3DXMATRIX&			GetAnimationTM();
			

	//  Object list
	// 자식 오브젝트를 추가한다.
	void				AttachChildNode(cSceneNode* pItem);
	void				DettachChildNode(cSceneNode* pItem);

	// 자신을 포함한 자식ASE오브젝트에서 ASE오브젝트를 찾는다.
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


	cSceneNode*			GetRootNode() const { return m_pRootNode; }
	void				SetRootNode(cSceneNode* val) { m_pRootNode = val; }



	BOOL				GetIsActiveAnimation() const { return m_bIsActiveAnimation; }
	void				SetIsActiveAnimation(BOOL val) { m_bIsActiveAnimation = val; }

	bool GetRender() const { return m_bRender; }
	void SetRender(bool val) { m_bRender = val; }
	void				UpdateChildren(DWORD elapseTime);
	void				RenderChildren();
	void				FreeChildren();

	void				SetBoundingSphere(cSphere& Sphere);
	cSphere&			GetBoundingSphere()  { return m_BoundingSphere; }

	//					순회 하면서 Renderer들을 큐에 넣는다.
	virtual void		CullRendererIntoRendererQueue(cView* pView,cCameraNode* pActiveCamera);
	
	cSceneNode&	operator=(const cSceneNode& other);


	virtual void		Render();
	virtual void		RenderShadow();
	virtual void		Update(DWORD elapseTime);;

	/*
		빌드란 씬노드구성후 작동하기전에 노드의 후처리 작업으로 완성시킴을 의미한다.
	*/
	virtual	void		BuildComposite();

	// ISerialize
	virtual void		SerializeIn(std::fstream& in);
	virtual void		SerializeOut(std::fstream& out);

	virtual void		QueueRenderer(cView* pView,bool bTraversal);

	
};

