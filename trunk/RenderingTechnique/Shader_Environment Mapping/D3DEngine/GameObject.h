#pragma once
#include "./Common/interface.h"

class cSphere;
class cScene;
class cLine;
class cPlane;
class cWorld;
class cMeshNode;
class cGameObject :
	public IUnknownObject,
	public cIProgressable,
	public cIRenderable
{
public:
	cGameObject(cWorld* pWorld);
	virtual ~cGameObject(void);

private:	
	D3DXMATRIX		m_WorldTM;			//���� ��, ��ġ����
	D3DXMATRIX		m_ScaleTM;
	D3DXMATRIX		m_RotationTM;
	D3DXMATRIX		m_TranslationTM;
	

	D3DXVECTOR3		m_Position;
	D3DXVECTOR3		m_PositionOld;		// ���� ��ġ
//	D3DXVECTOR3		m_PositionAcc;				

	float			m_Speed;			// �ӷ�
	float			m_SpeedOld;			
//	float			m_SpeedAcc;			

	D3DXVECTOR3		m_Forward;			// ����
	D3DXVECTOR3		m_ForwardOld;		

	D3DXVECTOR3		m_RelativeVelocity;	// ���ӵ� ��� �浹�� ���ӵ��� �˻��Ѵ�.

	cMeshNode*		m_pTarget;
	cWorld*			m_pWorld;			//��ü�� �����ϴ� ����

					//�ΰ�����(��������������������)
	cScene*			m_pScene;			//�� ����	
	cSphere*		m_pSphere;			//�� ���� 
	cPlane*			m_pPlane;			//��� ����

public:
	
	D3DXMATRIX&		GetWorldTM()  { return m_WorldTM; }

	//IObject
	virtual void	Init();
	virtual void	Uninit();
	virtual void	Update();
	virtual void	Render();
	
	//�ӷ�
	float			GetSpeed() const { return m_Speed; }
	void			SetSpeed(float val) { m_Speed = val; }
	float			GetSpeedOld() const { return m_SpeedOld; }
	void			SetSpeedOld(float val) { m_SpeedOld = val; }

	//����
	D3DXVECTOR3&	GetForward()  { return m_Forward; }
	void			SetForward(D3DXVECTOR3& val) { m_Forward=val; }
	D3DXVECTOR3&	GetForwardOld()  { return m_ForwardOld; }
	void			SetForwardOld(D3DXVECTOR3& val) { m_ForwardOld=val; }

	//��ġ
	void			SetPosition(D3DXVECTOR3& val);
	D3DXVECTOR3&	GetPosition();
	void			SetPositionOld(D3DXVECTOR3& val);
	D3DXVECTOR3&	GetPositionOld();

	//Ÿ��
	cMeshNode*		GetTarget() const { return m_pTarget; }
	void			SetTarget(cMeshNode* val) { m_pTarget = val; }
	
	//����
	cWorld*			GetWorld() const { return m_pWorld; }
	void			SetWorld(cWorld* val) { m_pWorld = val; }

	//���ӵ�
	const D3DXVECTOR3& GetRelativeVelocity() const { return m_RelativeVelocity; }
	void SetRelativeVelocity(const D3DXVECTOR3& val) { m_RelativeVelocity = val; }

	D3DXMATRIX&		MovePositionByLocalAxis( float deltaX,float deltaY,float deltaZ );
	D3DXMATRIX&		RotateByLocalAxis( float angleX,float angleY,float angleZ );

	// �ΰ�����
	void			CreateSphere();
	void			DestroySphere();
	cSphere*		GetSphere() const { return m_pSphere; }
	void			SetSphere(cSphere* val) { m_pSphere = val; }

	void			CreatePlane();
	void			DestroyPlane();
	cPlane*			GetPlane()  { return m_pPlane; }
	void			SetPlane(cPlane* val) { m_pPlane = val; }
	
	BOOL			LoadModel(LPCTSTR strFileName);
	cScene*		GetModel()  { return m_pScene; }	

	virtual void	OnCollide(cGameObject* pObj,cMeshNode* pMyMesh);
	virtual void	OnCollide(cGameObject* pObj);	
	virtual void	OnPicked(cMeshNode* pPickedMesh);


	
};

