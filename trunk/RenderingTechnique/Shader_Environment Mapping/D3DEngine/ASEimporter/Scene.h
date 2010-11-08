#pragma once
#include "SceneNode.h"
#include "../Common/interface.h"
#include "MaterialNode.h"


struct SCENEINFO 
{
	string			FILENAME;		// 모델의 파일이름
	DWORD			FIRSTFRAME;		// 시작 프레임
	DWORD			LASTFRAME;		// 마지막 프레임
	DWORD			LASTFRAMEMS;	// 마지막 프레임의 ms
	DWORD			FRAMESPEED;		// 초당 프레임수
	DWORD			TICKSPERFRAME;	// 프레임당 틱수
	DWORD			MESHFRAMESTEP;
	DWORD			KEYFRAMESTEP;
	D3DXVECTOR3		BACKGROUND_STATIC;
	D3DXVECTOR3		AMBIENT_STATIC;
	DWORD			EX_MSPERFRAME;		// 계산된 부가정보. 프레임당 ms
	DWORD			EX_TICKSPERMS;		// 계산된 부가정보. ms당 틱수	
	DWORD			EX_LASTFRAMEMS;		// 계산된 부가정보. 마지막 프레임 ms
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
	list<cSceneNode*>	m_listMeshObject;	//충돌검사하기위해 메쉬를 리스트 작성
	vector<cMaterialNode> m_arrMatrial;
	
	
	
	cSceneNode			m_RootASEObject;
	
	
	SCENEINFO			m_ModelInfo;
	BOOL				m_bActiveCamera;
	int					m_MeshRenderType;

	cSphere*			m_pBoundSphere;		//메쉬 전체를 감싸는 구
	

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


