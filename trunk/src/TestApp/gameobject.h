#pragma once
#include "../../Framework/IUnknown.h"

class cSphere;
class cSceneRoot;
class cLine;
class cPlane;
class cWorld;
class cMeshNode;
class cGameObject :
	public Game::IUnknown,
	public cIUpdatable,
	public IRenderable
{
public:
	cGameObject();
	virtual ~cGameObject(void);

private:	
	D3DXMATRIX		m_WorldTM;			//로컬 축, 위치정보
	D3DXMATRIX		m_ScaleTM;
	D3DXMATRIX		m_RotationTM;
	D3DXMATRIX		m_TranslationTM;
	

	D3DXVECTOR3		m_Position;
	D3DXVECTOR3		m_PositionOld;		// 이전 위치
//	D3DXVECTOR3		m_PositionAcc;				

	float			m_Speed;			// 속력
	float			m_SpeedOld;			
//	float			m_SpeedAcc;			

	D3DXVECTOR3		m_Forward;			// 전방
	D3DXVECTOR3		m_ForwardOld;		

	D3DXVECTOR3		m_RelativeVelocity;	// 상대속도 모든 충돌은 상대속도로 검사한다.

	cMeshNode*		m_pTarget;

					//부가정보(존재하지않을수도있음)
	cSceneRoot*		m_pRootNode;			//모델 정보	
	cSphere*		m_pSphere;			//구 정보 
	cPlane*			m_pPlane;			//평면 정보

public:
	
	D3DXMATRIX&		GetWorldTM()  { return m_WorldTM; }

	//IObject
	virtual void	Init();
	virtual void	Uninit();
	virtual void	Update(DWORD elapseTime);
	virtual void	Render();
	
	//속력
	float			GetSpeed() const { return m_Speed; }
	void			SetSpeed(float val) { m_Speed = val; }
	float			GetSpeedOld() const { return m_SpeedOld; }
	void			SetSpeedOld(float val) { m_SpeedOld = val; }

	//전방
	D3DXVECTOR3&	GetForward()  { return m_Forward; }
	void			SetForward(D3DXVECTOR3& val) { m_Forward=val; }
	D3DXVECTOR3&	GetForwardOld()  { return m_ForwardOld; }
	void			SetForwardOld(D3DXVECTOR3& val) { m_ForwardOld=val; }

	//위치
	void			SetPosition(D3DXVECTOR3& val);
	D3DXVECTOR3&	GetPosition();
	void			SetPositionOld(D3DXVECTOR3& val);
	D3DXVECTOR3&	GetPositionOld();

	//타겟
	cMeshNode*		GetTarget() const { return m_pTarget; }
	void			SetTarget(cMeshNode* val) { m_pTarget = val; }


	//상대속도
	const D3DXVECTOR3& GetRelativeVelocity() const { return m_RelativeVelocity; }
	void SetRelativeVelocity(const D3DXVECTOR3& val) { m_RelativeVelocity = val; }

	D3DXMATRIX&		MovePositionByLocalAxis( float deltaX,float deltaY,float deltaZ );
	D3DXMATRIX&		RotateByLocalAxis( float angleX,float angleY,float angleZ );

	// 부가정보
	void			CreateSphere();
	void			DestroySphere();
	cSphere*		GetSphere() const { return m_pSphere; }
	void			SetSphere(cSphere* val) { m_pSphere = val; }

	void			CreatePlane();
	void			DestroyPlane();
	cPlane*			GetPlane()  { return m_pPlane; }
	void			SetPlane(cPlane* val) { m_pPlane = val; }
	
	BOOL			LoadModel(LPCTSTR strFileName);
	cSceneRoot*			GetModel()  { return m_pRootNode; }	

	virtual void	OnCollide(cGameObject* pObj,cMeshNode* pMyMesh);
	virtual void	OnCollide(cGameObject* pObj);	
	virtual void	OnPicked(cMeshNode* pPickedMesh);


	
};


