#pragma once

/*
	m_LocalTM �� parentWorldTM �� �����Ͽ� m_WorldTM�� ��� Ŭ���� 
*/

class cSceneNode;
class cTransformable
{
public:
	cTransformable(void);
	~cTransformable(void);
protected:
	D3DXMATRIX				m_WorldTM;		//							Transform�� ���ؼ��� ���ŵȴ�. 
	D3DXMATRIX				m_LocalTM;


	BOOL					m_bLocalTMModified;
	BOOL					m_bWorldTMModified;


public:
	BOOL					GetWorldTMModified() const { return m_bWorldTMModified; }
	void					SetWorldTMModified(BOOL val) { m_bWorldTMModified = val; }

	void					SetWorldTM(const D3DXMATRIX& val) { m_WorldTM = val; }
	const D3DXMATRIX&		GetWorldTM() const { return m_WorldTM; }



	const D3DXMATRIX&		GetLocalTM() const { return m_LocalTM; }
	void					SetLocalTM(const D3DXMATRIX& val) {  m_LocalTM = val; m_bLocalTMModified=TRUE; }


	void					SetLocalPos( D3DXVECTOR3& pos );
	void					GetLocalPos( D3DXVECTOR3& pos );	
	void					GetWorldPos( D3DXVECTOR3& pos );
	void					SetWorldPos( D3DXVECTOR3& pos );

	
	void					MoveOnLocal( float deltaX,float deltaY,float deltaZ );
	// ȸ�� ��� ����
	void					RotateOnLocal( float angleX,float angleY,float angleZ );

	// ������� ����
	void					UpdateWorldTM(D3DXMATRIX* pRepLocalTM=NULL,cTransformable* pParent=NULL);
};
