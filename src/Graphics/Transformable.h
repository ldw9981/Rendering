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
	D3DXMATRIX				m_matWorld;		//							Transform�� ���ؼ��� ���ŵȴ�. 
	D3DXMATRIX				m_matLocal;
	D3DXMATRIX				m_matVelocityPos;
	D3DXMATRIX				m_matVelocityRot;
	
//	BOOL					m_bModifiedMatLocal;
//	BOOL					m_bModifiedMatWorld;
	public:
	cTransformable(void);
	~cTransformable(void);
public:
	void					SetWorldTM(const D3DXMATRIX& val) { m_matWorld = val; }
	D3DXMATRIX&				GetWorldTM() { return m_matWorld; }
	virtual D3DXMATRIX*		GetWorldMatrixPtr()	{ return &m_matWorld;  }
	

	D3DXMATRIX&				GetLocalTM() { return m_matLocal; }
	void					SetLocalTM(const D3DXMATRIX& val);


	void					SetLocalPos( D3DXVECTOR3& pos );
	void					GetLocalPos( D3DXVECTOR3& pos );	
	const D3DXVECTOR3*		GetLocalPos();

	void					GetForward( D3DXVECTOR3& out );
	const D3DXVECTOR3*		GetForward();

	void					GetUp( D3DXVECTOR3& out );
	const D3DXVECTOR3*		GetUp();

	void					GetWorldPosition( D3DXVECTOR3& pos );
	void					GetWorldPosition( D3DXVECTOR4& pos );
	void					SetWorldPos( D3DXVECTOR3& pos );
	virtual	 D3DXVECTOR3*	GetWorldPositionPtr();
	
	void					TranslateLocal( float deltaX,float deltaY,float deltaZ );
	void					RotateOnLocal( float angleX,float angleY,float angleZ );
};

}