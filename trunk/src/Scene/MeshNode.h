#pragma once
#include "../Framework/interface.h"
#include "../Math/Sphere.h"
#include "../D3D9/VertexBuffer.h"
#include "../D3D9/IndexBuffer.h"
#include "SceneGraphNode.h"
#include "MaterialNode.h"

class cMeshNode;
struct BONEREFINFO
{
	wstring strNodeName;
	cMeshNode* pBoneRef;		//본은 무조건 메쉬이다	
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
	public SceneGraphNode,
	public Framework::IRenderer,
	private SharedLPDIRECT3DDEVICE9
{
public:
	cMeshNode(void);
	virtual ~cMeshNode(void);

private:	
			

	vector<BONEREFINFO>		m_arrayBoneRef;				//메쉬가 참조하는 본 정보	
	map<SUBMATINDEX,WORD>	m_mapSubIndexCount;
	UINT					m_TotalBoneRef;

	D3DXMATRIX				m_BoneOffsetTM;
	D3DXMATRIX				m_BoneOffsetTMInv;

	BOOL					m_bIsBone;

	D3DXVECTOR3				m_vecVelocity;		// 속도	
	D3DXVECTOR3				m_WorldPosOld;		// 이전 위치

	UINT					m_TotalVertex;

	cMaterialEx				m_Matrial;
	IndexBuffer*		m_pIndexBuffer;	
	VertexBuffer*		m_pVetextBuffer;	
	
public:

	UINT 					GetTotalBoneRef() const { return m_TotalBoneRef; }
	void 					SetTotalBoneRef(UINT val) { m_TotalBoneRef = val; }

	UINT 					GetTotalVertex() const { return m_TotalVertex; }
	void 					SetTotalVertex(UINT val) { m_TotalVertex = val; }

	// SceneGraphNode
	vector<BONEREFINFO>&	GetArrayBoneRef()  { return m_arrayBoneRef; }
	map<SUBMATINDEX,WORD>&	GetMapSubIndexCount()  { return m_mapSubIndexCount; }
	IndexBuffer*			GetIndexBuffer() const { return m_pIndexBuffer; }
	void					SetIndexBuffer(IndexBuffer* val) { m_pIndexBuffer = val; }
	
	VertexBuffer*			GetVetextBuffer() const { return m_pVetextBuffer; }
	void					SetVetextBuffer(VertexBuffer* val) { m_pVetextBuffer = val; }

	// 참조하는 본의 노드네임을 추가한다.
	void					InsertBoneRefName(LPCWSTR strNodeName);
	
	// 루트에서 부터 본을 찾아 찾아 연결한다.(본의 연결은 모든 본을 게층구조에 넣은후에 연결한다.) 
	void					LinkToBone();


	BOOL					GetIsBone() const { return m_bIsBone; }
	void					SetIsBone(BOOL val) { m_bIsBone = val; }		

	BOOL					IsSkinnedMesh();


	const D3DXVECTOR3&		GetVelocity()  { return m_vecVelocity; }

	void					SetMatrial(cMaterialEx& val) { m_Matrial = val; }	




	virtual void			Init();
	virtual void			Uninit();

	virtual void			Update(DWORD elapseTime);
	virtual void			Render();
	
};



