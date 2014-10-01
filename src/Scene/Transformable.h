#pragma once
namespace Sophia
{

/*
	m_LocalTM �� parentWorldTM �� �����Ͽ� m_WorldTM�� ��� Ŭ���� 
	��ȿ�� ���� �κ��� ���� ����ȭ�� ������ ����.
*/

class cSceneNode;
class cTransformable
{
public:
	cTransformable(void);
	~cTransformable(void);
protected:
	D3DXMATRIX				m_matWorld;		//							Transform�� ���ؼ��� ���ŵȴ�. 
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



	D3DXMATRIX&				GetLocalTM() { return m_matLocal; }
	void					SetLocalTM(const D3DXMATRIX& val);


	void					SetLocalPos( D3DXVECTOR3& pos );
	void					GetLocalPos( D3DXVECTOR3& pos );	
	const D3DXVECTOR3*		GetLocalPos();

	void					GetWorldPos( D3DXVECTOR3& pos );
	void					SetWorldPos( D3DXVECTOR3& pos );
	const D3DXVECTOR3*		GetWorldPos();

	
	void					MoveOnLocal( float deltaX,float deltaY,float deltaZ );
	// ȸ�� ��� ����
	void					RotateOnLocal( float angleX,float angleY,float angleZ );

	// ������� ����
	virtual void			UpdateWorldMatrix(cTransformable* pParent=NULL);
	//void					Update(DWORD dwElapseMS);

	D3DXVECTOR3				GetVelPosPerSec()  { return m_velPosPerSec; }
	D3DXVECTOR3				GetVelRotPerSec()  { return m_velRotPerSec; }
	void					SetVelocityPosition( float x,float y,float z );
	void					SetVelocityPosition( D3DXVECTOR3& pos );
	void					SetVelocityRotation( float x,float y,float z );
	void					SetVelocityRotation( D3DXVECTOR3& rot );
};

}