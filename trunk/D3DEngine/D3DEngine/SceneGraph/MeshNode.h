#pragma once
#include "../Common/interface.h"
#include "../Geometrics/Sphere.h"
#include "../Resource/RscVertexBuffer.h"
#include "../Resource/RscIndexBuffer.h"
#include "SceneGraphNode.h"
#include "MaterialNode.h"

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
	public cSceneGraphNode,
	public IRenderer,
	private cStaticD3DDEVICE9
{
public:
	cMeshNode(void);
	virtual ~cMeshNode(void);
private:	
	vector<BONEREFINFO>		m_arrayBoneRef;			// fixed �޽��� �����ϴ� �� ���� 
	map<SUBMATINDEX,WORD>	m_mapSubIndexCount;		// fixed
	UINT					m_TotalBoneRef;			// fixed
	BOOL					m_bIsBone;				// fixed
	UINT					m_TotalVertex;			// fixed 
	cMaterialEx				m_Matrial;				// fixed
	cRscIndexBuffer*		m_pRscIndexBuffer;		// fixed 
	cRscVertexBuffer*		m_pRscVetextBuffer;		// fixed 
	
	D3DXVECTOR3				m_vecVelocity;			// dynamic �ӵ�			
	D3DXVECTOR3				m_WorldPosOld;			// dynamic ���� ��ġ 
public:

	UINT 					GetTotalBoneRef() const { return m_TotalBoneRef; }
	void 					SetTotalBoneRef(UINT val) { m_TotalBoneRef = val; }

	UINT 					GetTotalVertex() const { return m_TotalVertex; }
	void 					SetTotalVertex(UINT val) { m_TotalVertex = val; }

	// cSceneGraphNode
	vector<BONEREFINFO>&	GetArrayBoneRef()  { return m_arrayBoneRef; }
	map<SUBMATINDEX,WORD>&	GetMapSubIndexCount()  { return m_mapSubIndexCount; }
	cRscIndexBuffer*		GetRscIndexBuffer() const { return m_pRscIndexBuffer; }
	void					SetRscIndexBuffer(cRscIndexBuffer* val) { m_pRscIndexBuffer = val; }
	
	cRscVertexBuffer*		GetRscVetextBuffer() const { return m_pRscVetextBuffer; }
	void					SetRscVetextBuffer(cRscVertexBuffer* val) { m_pRscVetextBuffer = val; }

	// �����ϴ� ���� �������� �߰��Ѵ�.
	void					InsertBoneRefName(LPCWSTR strNodeName);
	
	// ��Ʈ���� ���� ���� ã�� ã�� �����Ѵ�.(���� ������ ��� ���� ���������� �����Ŀ� �����Ѵ�.) 
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



