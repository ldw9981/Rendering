#pragma once

/*
	m_LocalTM �� parentWorldTM �� �����Ͽ� m_WorldTM�� ��� Ŭ���� 
*/

class cSceneGraphNode;
class cTransformable
{
public:
	cTransformable(void);
	~cTransformable(void);
public:
	D3DXMATRIX				m_WorldInverseTM;
	D3DXMATRIX				m_WorldTM;		//							Transform�� ���ؼ��� ���ŵȴ�. 

	D3DXMATRIX				m_LocalTM;
	D3DXMATRIX				m_TranslationTM;
	D3DXMATRIX				m_RotationTM;
	D3DXMATRIX				m_ScaleTM;

	D3DXVECTOR3				m_WorldPos;
private:
	BOOL					m_bLocalTMModified;
	BOOL					m_bWorldTMModified;


public:
	BOOL GetWorldTMModified() const { return m_bWorldTMModified; }
	void SetWorldTMModified(BOOL val) { m_bWorldTMModified = val; }

	void					SetWorldTM(const D3DXMATRIX& val) { m_WorldTM = val; }
	const D3DXMATRIX&		GetWorldTM() const { return m_WorldTM; }

	const D3DXMATRIX&		GetWorldInverseTM();

	const D3DXMATRIX&		GetLocalTM() const { return m_LocalTM; }
	void					SetLocalTM(const D3DXMATRIX& val) {  m_LocalTM = val; m_bLocalTMModified=TRUE; }


	void					SetLocalPos( D3DXVECTOR3& pos );
	void					GetLocalPos( D3DXVECTOR3& pos );	
	const D3DXVECTOR3&		GetWorldPos();
	void					SetWorldPos( D3DXVECTOR3& pos );

	
	void					MoveOnLocal( float deltaX,float deltaY,float deltaZ );
	// ȸ�� ��� ����
	void					RotateOnLocal( float angleX,float angleY,float angleZ );

	// ������� ����
	BOOL					UpdateWorldTM(D3DXMATRIX* pRepLocalTM=NULL,cTransformable* pParent=NULL);
};
