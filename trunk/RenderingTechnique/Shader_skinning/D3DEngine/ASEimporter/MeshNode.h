#pragma once
#include "../Common/interface.h"
#include "../Geometrics/Sphere.h"
#include "../Renderer/D3DGraphics.h"
#include "../Resource/RscVertexBuffer.h"
#include "../Resource/RscIndexBuffer.h"
#include "SceneNode.h"
#include "../ASEimporter/MaterialNode.h"

class cMeshNode;
struct BONEREFINFO
{
	wstring strNodeName;
	cMeshNode* pBoneRef;		//���� ������ �޽��̴�	
	D3DXMATRIX	BoneOffSetTM_INV;
};






struct BONEWEIGHT
{
	BYTE	bone_index;
	float	bone_weight;		

	static BOOL GreatBoneWeight(BONEWEIGHT* a,BONEWEIGHT* b)
	{
		if ( a->bone_weight > b->bone_weight)
			return TRUE;

		return FALSE;
	}
};



class cMeshNode:
	public cSceneNode,
	private cStaticD3DDEVICE9,
	private cStaticD3DGraphics
{
public:
	cMeshNode(void);
	virtual ~cMeshNode(void);

private:	

	vector<BONEREFINFO>				m_arrayBoneRef;				//�޽��� �����ϴ� �� ����	
	map<SUBMATINDEX,WORD>			m_mapSubIndexCount;
	UINT							m_TotalBoneRef;

	D3DXMATRIX						m_BoneOffsetTM;
	D3DXMATRIX						m_BoneOffsetTMInv;

	cSphere*						m_pBoundingSphere;	
	BOOL							m_bIsBone;

	D3DXVECTOR3						m_vecVelocity;		// �ӵ�	
	D3DXVECTOR3						m_Position;
	D3DXVECTOR3						m_PositionOld;		// ���� ��ġ

	UINT							m_TotalVertex;

	cMaterialNode					m_Matrial;
	cRscIndexBuffer*				m_pRscIndexBuffer;	
	cRscVertexBuffer*				m_pRscVetextBuffer;	
	
public:

	UINT GetTotalBoneRef() const { return m_TotalBoneRef; }
	void SetTotalBoneRef(UINT val) { m_TotalBoneRef = val; }

	UINT GetTotalVertex() const { return m_TotalVertex; }
	void SetTotalVertex(UINT val) { m_TotalVertex = val; }

	// cSceneNode
	vector<BONEREFINFO>&	GetArrayBoneRef()  { return m_arrayBoneRef; }
	map<SUBMATINDEX,WORD>&	GetMapSubIndexCount()  { return m_mapSubIndexCount; }
	cRscIndexBuffer*		GetRscIndexBuffer() const { return m_pRscIndexBuffer; }
	void					SetRscIndexBuffer(cRscIndexBuffer* val) { m_pRscIndexBuffer = val; }
	
	cRscVertexBuffer*		GetRscVetextBuffer() const { return m_pRscVetextBuffer; }
	void					SetRscVetextBuffer(cRscVertexBuffer* val) { m_pRscVetextBuffer = val; }

	// �����ϴ� ���� �������� �߰��Ѵ�.
	void				InsertBoneRefName(LPCWSTR strNodeName);
	
	// ��Ʈ���� ���� ���� ã�� ã�� �����Ѵ�.(���� ������ ��� ���� ���������� �����Ŀ� �����Ѵ�.) 
	void				LinkToBone();


	BOOL				GetIsBone() const { return m_bIsBone; }
	void				SetIsBone(BOOL val) { m_bIsBone = val; }		

	BOOL				IsSkinnedMesh();

//	cSphere*			GetBoundingSphere();
//	void				CreateBoundingSphere();

	const D3DXVECTOR3&	GetVelocity()  { return m_vecVelocity; }
	const D3DXVECTOR3&	GetPosition()  { return m_Position; }

	void				SetMatrial(cMaterialNode val) { m_Matrial = val; }	

	virtual void		Init();
	virtual void		Uninit();

	virtual void		Update(DWORD elapseTime);
	virtual void		Render();

};



