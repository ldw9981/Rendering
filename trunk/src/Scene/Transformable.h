#pragma once
namespace Sophia
{

/*
	m_LocalTM 과 parentWorldTM 을 참조하여 m_WorldTM을 얻는 클래스 
	비효율 적인 부분이 많아 최적화할 여지가 많다.
*/

class cSceneNode;
class cTransformable
{
public:
	cTransformable(void);
	~cTransformable(void);
public:
	D3DXMATRIX				m_matWorld;		//							Transform에 의해서만 갱신된다. 
	D3DXMATRIX				m_matLocal;
	D3DXMATRIX				m_matVelocityPos;
	D3DXMATRIX				m_matVelocityRot;
	
//	BOOL					m_bModifiedMatLocal;
//	BOOL					m_bModifiedMatWorld;

	bool					m_bNoVelocityPos;
	bool					m_bNoVelocityRot;
	D3DXVECTOR3				m_velPosPerSec;

	D3DXVECTOR3				m_velRotPerSec;
	D3DXVECTOR3				m_velSclPerSec;

public:
//	BOOL					GetWorldTMModified() const { return m_bModifiedMatWorld; }
//	void					SetWorldTMModified(BOOL val) { m_bModifiedMatWorld = val; }

	void					SetWorldTM(const D3DXMATRIX& val) { m_matWorld = val; }
	D3DXMATRIX&				GetWorldTM() { return m_matWorld; }
	virtual D3DXMATRIX*		GetWorldMatrixPtr()	{ return &m_matWorld;  }
	

	D3DXMATRIX&				GetLocalTM() { return m_matLocal; }
	void					SetLocalTM(const D3DXMATRIX& val);


	void					SetLocalPos( D3DXVECTOR3& pos );
	void					GetLocalPos( D3DXVECTOR3& pos );	
	const D3DXVECTOR3*		GetLocalPosPtr();

	void					GetForward( D3DXVECTOR3& out );
	const D3DXVECTOR3*		GetForwardPtr();

	void					GetWorldPosition( D3DXVECTOR3& pos );
	void					GetWorldPosition( D3DXVECTOR4& pos );
	void					SetWorldPos( D3DXVECTOR3& pos );
	virtual	 D3DXVECTOR3*	GetWorldPositionPtr();
	
	void					MoveOnLocal( float deltaX,float deltaY,float deltaZ );
	// 회전 행렬 리턴
	void					RotateOnLocal( float angleX,float angleY,float angleZ );

	D3DXVECTOR3				GetVelPosPerSec()  { return m_velPosPerSec; }
	D3DXVECTOR3				GetVelRotPerSec()  { return m_velRotPerSec; }
	void					SetVelocityPosition( float x,float y,float z );
	void					SetVelocityPosition( D3DXVECTOR3& pos );
	void					SetVelocityRotation( float x,float y,float z );
	void					SetVelocityRotation( D3DXVECTOR3& rot );
};

}