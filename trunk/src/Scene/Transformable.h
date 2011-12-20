#pragma once

/*
	m_LocalTM 과 parentWorldTM 을 참조하여 m_WorldTM을 얻는 클래스 
*/

class cSceneNode;
class cTransformable
{
public:
	cTransformable(void);
	~cTransformable(void);
protected:
	D3DXMATRIX				m_matWorld;		//							Transform에 의해서만 갱신된다. 
	D3DXMATRIX				m_matLocal;
	D3DXMATRIX				m_matVelocityPos;
	D3DXMATRIX				m_matVelocityRot;
	
	BOOL					m_bModifiedMatLocal;
	BOOL					m_bModifiedMatWorld;
	BOOL					m_bModifiedVelocityPos;
	BOOL					m_bModifiedVelocityRot;
	D3DXVECTOR3				m_velPosPerSec;
	D3DXVECTOR3				m_velRotPerSec;
	D3DXVECTOR3				m_velSclPerSec;

public:
	BOOL					GetWorldTMModified() const { return m_bModifiedMatWorld; }
	void					SetWorldTMModified(BOOL val) { m_bModifiedMatWorld = val; }

	void					SetWorldTM(const D3DXMATRIX& val) { m_matWorld = val; }
	const D3DXMATRIX&		GetWorldTM() const { return m_matWorld; }



	const D3DXMATRIX&		GetLocalTM() const { return m_matLocal; }
	void					SetLocalTM(const D3DXMATRIX& val) {  m_matLocal = val; m_bModifiedMatLocal=TRUE; }


	void					SetLocalPos( D3DXVECTOR3& pos );
	void					GetLocalPos( D3DXVECTOR3& pos );	
	void					GetWorldPos( D3DXVECTOR3& pos );
	void					SetWorldPos( D3DXVECTOR3& pos );

	
	void					MoveOnLocal( float deltaX,float deltaY,float deltaZ );
	// 회전 행렬 리턴
	void					RotateOnLocal( float angleX,float angleY,float angleZ );

	// 월드행렬 리턴
	void					UpdateWorldMatrix(D3DXMATRIX* pRepLocalTM=NULL,cTransformable* pParent=NULL);
	void					Update(DWORD dwElapseMS);


	void					SetVelocityPosition( float x,float y,float z );
	void					SetVelocityRotation( float x,float y,float z );
};
