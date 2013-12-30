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
class Frustum;
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

	// �Ʒ��ΰ��� ���ؽ� ���۰� �������� ���������.
	cSphere*				m_pCullingSphere;		// �ڽ��� �ø��� �� ��� �����ϴ±�(��ġ,������ �������� ���ŵ�) 
	cSphere					m_BoundingSphere;		// �⺻ �� (�ѹ��������ϸ� �������� ��ġ�� ����)
	
	// Transform �ִϸ��̼� ����
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

	//					��ȸ �ϸ鼭 Renderer���� ť�� �ִ´�.
	virtual void		CullRendererIntoRendererQueue(cView* pView,Frustum* pFrustum );
	
	cSceneNode&	operator=(const cSceneNode& other);


	virtual void		Render();
	virtual void		RenderShadow();
	virtual void		Update(DWORD elapseTime);;

	/*
		����� ����屸���� �۵��ϱ����� ����� ��ó�� �۾����� �ϼ���Ŵ�� �ǹ��Ѵ�.
	*/
	virtual	void		BuildComposite();

	// ISerialize
	virtual void		SerializeIn(std::fstream& in);
	virtual void		SerializeOut(std::fstream& out);

	virtual void		QueueRenderer(cView* pView,bool bTraversal);
	virtual void		QueueRendererShadow(cView* pView,bool bTraversal);
	
};

