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
class cSceneRoot :
	public cSceneNode
{
public:
	cSceneRoot(void);
	virtual ~cSceneRoot(void);

	
private:
	cGameObject*			m_pOwner;

	vector<cMaterialNode>	m_arrMatrial;			
		
	SCENEINFO				m_ModelInfo;
	BOOL					m_bActiveCamera;

	cSphere*				m_pBoundSphere;		//�޽� ��ü�� ���δ� ��
	
	BOOL					m_bCompleteBoundBox;	
	D3DXVECTOR3				m_BoundBoxMin;
	D3DXVECTOR3				m_BoundBoxMax;

public:

//	void					InsertSceneNode(cSceneNode* pItem);
//	cSceneNode*				FindSceneNode(LPCSTR nodename,int type=cSceneNode::ANYTHING);

	virtual	void			Init();
	virtual	void			Uninit();
	virtual	void			Update(DWORD elapseTime);
	virtual	void			Render();

	void					InsertMatrial(cMaterialNode& Matrial);
	cMaterialNode*			GetMatrial( UINT index );


	cSphere*				GetBoundSphere()  { return m_pBoundSphere; }
	SCENEINFO&				GetSceneInfo()  { return m_ModelInfo; }	
	BOOL					GetActiveCamera() const { return m_bActiveCamera; }
	void					SetActiveCamera(BOOL val) { m_bActiveCamera = val; }
	
	cGameObject*			GetOwner() const { return m_pOwner; }
	void					SetOwner(cGameObject* val) { m_pOwner = val; }
		

	void					SetBoundBoxMin(D3DXVECTOR3& min);
	D3DXVECTOR3&			GetBoundBoxMin()  { return m_BoundBoxMin; }
	void					SetBoundBoxMax(D3DXVECTOR3& max);
	D3DXVECTOR3&			GetBoundBoxMax()  { return m_BoundBoxMax; }
	void					MakeBoundSphere();
	

	void					GetPositionByLocalAxis(D3DXVECTOR3& pos);
	void					SetPositionByLocalAxis(D3DXVECTOR3& pos);
};


