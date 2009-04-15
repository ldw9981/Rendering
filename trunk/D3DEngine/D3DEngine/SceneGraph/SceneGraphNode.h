#pragma once
#include "../Common/interface.h"
#include "../Geometrics/Transformable.h"


struct SCENEINFO; 
class cASEParser; 
class cRscTransformAnm;
class cRendererQueue;
class cSphere;
class cCameraNode;

class cSceneGraphNode:
	public IUnknownObject,
	public IUpdatable,	
	public cTransformable
{
public:
	cSceneGraphNode(void);
	virtual ~cSceneGraphNode(void);

enum ASEOBJ { ROOT,MESH,HELPER,LIGHT,CAMEARA,SHAPE,ANYTHING };
	
protected:
	cSphere*				m_pCullingSphere;		// 자식의 컬링구 를 모두 포함하는구(위치,볼륨을 매프레임 갱신해야함) 
	cSphere*				m_pBoundingSphere;		// 기본 구 (한번만설정하며 매프레임 위치만 갱신)

	// 노드 정보
	UINT					m_NodeType;	
	wstring					m_strNodeName;
	D3DXMATRIX				m_NodeTM;	
	wstring					m_strParentName;
	list<cSceneGraphNode*>	m_listChildNode;
	cSceneGraphNode*		m_pParentNode;
	cSceneGraphNode*		m_pRootNode;
		
	
	// Transform 애니메이션 정보
	cRscTransformAnm*		m_pRscTransformAnm;	
	UINT					m_TotalTransformAnm;
	UINT					m_IndexTransformAnm;
	DWORD					m_AccumTime;
	BOOL					m_bIsActiveAnimation;
	D3DXMATRIX				m_AnimationTM;			

	list<cSceneGraphNode*>::iterator m_ParentListIt;

public:	
	cRscTransformAnm*	GetRscTransformAnm() const { return m_pRscTransformAnm; }
	void				SetRscTransformAnm(cRscTransformAnm* val);

	UINT				GetTotalTransformAnm() const { return m_TotalTransformAnm; }
	void				SetTotalTransformAnm(UINT val) { m_TotalTransformAnm = val; }
	
	D3DXMATRIX*			UpdateTransformAnm(DWORD elapseTime);
	D3DXMATRIX&			GetAnimationTM();
			

	//  Object list
	// 자식 오브젝트를 추가한다.
	void				AttachChildNode(cSceneGraphNode* pItem);
	void				DettachChildNode(cSceneGraphNode* pItem);

	// 자신을 포함한 자식ASE오브젝트에서 ASE오브젝트를 찾는다.
	cSceneGraphNode*	FindNode(wstring& nodename,int type=ANYTHING);

	//Get/Set
	void				SetNodeName( LPCWSTR nodename );
	wstring&			GetNodeName();

	void				SetParentName( LPCWSTR nodename );
	wstring&			GetParentName();
	BOOL				IsExistParentName();		
	BOOL				IsRootNode();
	
		
	D3DXMATRIX&			GetNodeTM() { return m_NodeTM; }
	void				SetNodeTM(D3DXMATRIX &val) { m_NodeTM=val; }
		

	UINT				GetNodeType() const { return m_NodeType; }
	void				SetNodeType(UINT val) { m_NodeType = val; }

	cSceneGraphNode*	GetParentNode() const { return m_pParentNode; }
	void				SetParentNode(cSceneGraphNode* val) { m_pParentNode = val; }


	BOOL				LinkToParent();

	cSceneGraphNode*	GetRootNode() const { return m_pRootNode; }
	void				SetRootNode(cSceneGraphNode* val) { m_pRootNode = val; }



	BOOL				GetIsActiveAnimation() const { return m_bIsActiveAnimation; }
	void				SetIsActiveAnimation(BOOL val) { m_bIsActiveAnimation = val; }


	void				InitChildren();
	void				UninitChildren();
	void				UpdateChildren(DWORD elapseTime);
	void				RenderChildren();
	void				FreeChildren();

	cSphere*			GetCullingSphere() const { return m_pCullingSphere; }
	cSphere*			CreateCullingSphere();

	cSphere*			GetBoundingSphere() const { return m_pBoundingSphere; }
	cSphere*			CreateBoundingSphere();

	void				UpdateParentCullingSphere(cSphere& Sphere);

	virtual void		CullRendererTraversal(cRendererQueue* pRendererQueue,cCameraNode* pActiveCamera);
	void				InsertToRendererQueueTraversal( cRendererQueue* pRendererQueue,cCameraNode* pActiveCamera,WORD testPlane=0 );
	
	cSceneGraphNode&	operator=(const cSceneGraphNode& other);


	virtual void		Init()=0;
	virtual void		Uninit()=0;
	virtual void		Update(DWORD elapseTime)=0;
};

