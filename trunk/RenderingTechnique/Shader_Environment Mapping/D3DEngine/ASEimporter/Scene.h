#pragma once
#include "SceneNode.h"
#include "../Common/interface.h"
#include "MaterialNode.h"


struct SCENEINFO 
{
	string			FILENAME;		// ���� �����̸�
	DWORD			FIRSTFRAME;		// ���� ������
	DWORD			LASTFRAME;		// ������ ������
	DWORD			LASTFRAMEMS;	// ������ �������� ms
	DWORD			FRAMESPEED;		// �ʴ� �����Ӽ�
	DWORD			TICKSPERFRAME;	// �����Ӵ� ƽ��
	DWORD			MESHFRAMESTEP;
	DWORD			KEYFRAMESTEP;
	D3DXVECTOR3		BACKGROUND_STATIC;
	D3DXVECTOR3		AMBIENT_STATIC;
	DWORD			EX_MSPERFRAME;		// ���� �ΰ�����. �����Ӵ� ms
	DWORD			EX_TICKSPERMS;		// ���� �ΰ�����. ms�� ƽ��	
	DWORD			EX_LASTFRAMEMS;		// ���� �ΰ�����. ������ ������ ms
};



class cSphere;
class cGameObject;
class cScene :
	public IUnknownObject,
	public cIProgressable,
	public cIRenderable
{
public:
	cScene(void);
	virtual ~cScene(void);

	enum MESHRENDERTYPE {ONLYBONE,ONLYNORMALMESH,ONLYSKINEDMESH,ALL};
private:
	cGameObject*		m_pOwner;

	list<cSceneNode*>	m_listASEObject;
	list<cSceneNode*>	m_listMeshObject;	//�浹�˻��ϱ����� �޽��� ����Ʈ �ۼ�
	vector<cMaterialNode> m_arrMatrial;
	
	
	
	cSceneNode			m_RootASEObject;
	
	
	SCENEINFO			m_ModelInfo;
	BOOL				m_bActiveCamera;
	int					m_MeshRenderType;

	cSphere*			m_pBoundSphere;		//�޽� ��ü�� ���δ� ��
	

	BOOL				m_bCompleteBoundBox;	
	D3DXVECTOR3			m_BoundBoxMin;
	D3DXVECTOR3			m_BoundBoxMax;

public:

	void				InsertSceneNode(cSceneNode* pItem);
	cSceneNode*			FindSceneNode(LPCSTR nodename,int type=cSceneNode::ANYTHING);

	virtual	void		Init();
	virtual	void		Uninit();
	virtual	void		Update();
	virtual	void		Render();

	void				InsertMatrial(cMaterialNode& Matrial);
	cMaterialNode*		GetMatrial( UINT index );

	list<cSceneNode*>&  GetListMeshObject()  { return m_listMeshObject; }
	cSphere*			GetBoundSphere()  { return m_pBoundSphere; }
	SCENEINFO&			GetSceneInfo()  { return m_ModelInfo; }	
	BOOL				GetActiveCamera() const { return m_bActiveCamera; }
	void				SetActiveCamera(BOOL val) { m_bActiveCamera = val; }
	int					GetMeshRenderType() const { return m_MeshRenderType; }
	void				SetMeshRenderType(int val) { m_MeshRenderType = val; }
	cSceneNode&			GetRootASEObject()  { return m_RootASEObject; }
	cGameObject*		GetOwner() const { return m_pOwner; }
	void				SetOwner(cGameObject* val) { m_pOwner = val; }
		

	void				SetBoundBoxMin(D3DXVECTOR3& min);
	D3DXVECTOR3&		GetBoundBoxMin()  { return m_BoundBoxMin; }
	void				SetBoundBoxMax(D3DXVECTOR3& max);
	D3DXVECTOR3&		GetBoundBoxMax()  { return m_BoundBoxMax; }
	void				MakeBoundSphere();
	

	void				GetPositionByLocalAxis(D3DXVECTOR3& pos);
	void				SetPositionByLocalAxis(D3DXVECTOR3& pos);
	D3DXMATRIX&			MovePositionByLocalAxis( float deltaX,float deltaY,float deltaZ );
	D3DXMATRIX&			RotateByLocalAxis( float angleX,float angleY,float angleZ );
};


