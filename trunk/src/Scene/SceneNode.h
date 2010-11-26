#pragma once
#include "Foundation/interface.h"
#include "Scene/Transformable.h"


struct SCENEINFO; 
class cASEParser; 
class cRscTransformAnm;
class cRendererQueue;
class cSphere;
class cCameraNode;
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
	string					m_strNodeName;			
	string					m_strParentName;		
	list<cSceneNode*>		m_listChildNode;		
	cSceneNode*				m_pParentNode;
	cSceneNode*				m_pRootNode;
	cSphere*				m_pCullingSphere;		// �ڽ��� �ø��� �� ��� �����ϴ±�(��ġ,������ �������� �����ؾ���) 
	cSphere*				m_pBoundingSphere;		// �⺻ �� (�ѹ��������ϸ� �������� ��ġ�� ����)
	
	// Transform �ִϸ��̼� ����
	cRscTransformAnm*		m_pRscTransformAnm;	

	BOOL					m_bIsActiveAnimation;
	D3DXMATRIX				m_AnimationTM;			
	bool					m_bRender;

	list<cSceneNode*>::iterator m_ParentListIt;

public:	
	void				SetNodeInfo(SCENENODEINFO& stInfo);
	cRscTransformAnm*	GetRscTransformAnm() const { return m_pRscTransformAnm; }
	void				SetRscTransformAnm(cRscTransformAnm* val);
		
	D3DXMATRIX*			UpdateTransformAnm(DWORD elapseTime);
	D3DXMATRIX&			GetAnimationTM();
			

	//  Object list
	// �ڽ� ������Ʈ�� �߰��Ѵ�.
	void				AttachChildNode(cSceneNode* pItem);
	void				DettachChildNode(cSceneNode* pItem);

	// �ڽ��� ������ �ڽ�ASE������Ʈ���� ASE������Ʈ�� ã�´�.
	cSceneNode*			FindNode(string& nodename);

	//Get/Set
	void				SetNodeName( const char* nodename );
	string&				GetNodeName();

	void				SetParentName( const char* nodename );
	string&				GetParentName();
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

	void				SetCullingSphere(cSphere& Sphere);
	cSphere*			GetCullingSphere() const { return m_pCullingSphere; }
	cSphere*			CreateCullingSphere();

	void				SetBoundingSphere(cSphere& Sphere);
	cSphere*			GetBoundingSphere() const { return m_pBoundingSphere; }
	cSphere*			CreateBoundingSphere();

	void				UpdateParentCullingSphere(cSphere& Sphere);

	virtual void		CullRendererTraversal(cRendererQueue* pRendererQueue,cCameraNode* pActiveCamera);
	void				PushTraversal( cRendererQueue* pRendererQueue,cCameraNode* pActiveCamera,WORD testPlane=0 );
	
	cSceneNode&	operator=(const cSceneNode& other);


	virtual void		Render()=0;
	virtual void		Update(DWORD elapseTime)=0;

	/*
		����� ����屸���� �۵��ϱ����� ����� ��ó�� �۾����� �ϼ���Ŵ�� �ǹ��Ѵ�.
	*/
	virtual	void		BuildComposite();

	// ISerialize
	virtual void		SerializeIn(fstream& in);
	virtual void		SerializeOut(fstream& out);
};

