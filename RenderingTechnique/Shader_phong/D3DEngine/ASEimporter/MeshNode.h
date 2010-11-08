#pragma once
#include "../Common/interface.h"
#include "../Geometrics/Sphere.h"
#include "../Renderer/D3DGraphics.h"
#include "../Resource/RscIndexBuffer.h"
#include "SceneNode.h"
#include "../ASEimporter/MaterialNode.h"

class cMeshNode;
struct BONEREFINFO
{
	string strNodeName;
	cMeshNode* pBoneRef;		//���� ������ �޽��̴�
};


struct TEXCOORD:public pair<float,float>
{
	TEXCOORD()
	{
		first=0.0f; second=0.0f;
	}
	TEXCOORD(float u,float v)
	{
		first=u; second=v;
	}
};

struct NORMALVERTEX
{	
	D3DXVECTOR3	vertex;		//x,y,z
	D3DXVECTOR3 normal;		//normal
	TEXCOORD	tex;
};


struct BLENDVERTEX
{
	//x,y,z
	D3DXVECTOR3	vertex;
	// weight
	float		weight0;
	float		weight1;
	float		weight2;
	// index
	BYTE		index0;
	BYTE		index1;
	BYTE		index2;
	BYTE		index3;
	//normal
	D3DXVECTOR3 normal;
	TEXCOORD	tex;
};


#define FVF_NORMALVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)  
#define FVF_BLENDVERTEX (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL|D3DFVF_TEX1)


struct VNORMAL
{
	WORD iRefFace;
	WORD iRefVertex;
	D3DXVECTOR3 normal;
};



typedef WORD SUBMATINDEX;

struct BONEWEIGHT
{
	BYTE	bone_index;
	float	bone_weight;		
};



class cMeshNode 
	:public cSceneNode,private cStaticD3DDEVICE9,private cStaticD3DGraphics
{
public:
	cMeshNode(void);
	virtual ~cMeshNode(void);

private:	
	LPDWORD							m_pArVertex;
	LPWORD							m_pArIndex;
	DWORD							m_dwByteVertex;
	DWORD							m_dwByteIndex;


	vector<pair<FACEINDEX,SUBMATINDEX>>	m_arrFaceIndex;
	vector<NORMALVERTEX>			m_arrayNormalVertex;		//���� ���ؽ�
	vector<BLENDVERTEX>				m_arrayBlendVertex;			//���ؽ� ����
	vector<BONEREFINFO>				m_arrayBoneRef;				//�޽��� �����ϴ� �� ����


	vector<pair<SUBMATINDEX,WORD>>    m_listSubIndexCount;
	map<SUBMATINDEX,WORD>		m_mapSubIndexCount;

	int		m_NUMFACES;
	int		m_NUMVERTEX;	
	int		m_NUMBONEREF;



	D3DXMATRIX						m_BoneOffsetTM;
	
	LPDIRECT3DVERTEXBUFFER9			m_pVB; 
	LPDIRECT3DINDEXBUFFER9			m_pIB;
	

	cSphere*						m_pBoundingSphere;
	
	BOOL							m_bIsBone;

	D3DXVECTOR3						m_vecVelocity;		// �ӵ�	
	D3DXVECTOR3						m_Position;
	D3DXVECTOR3						m_PositionOld;		// ���� ��ġ


	cMaterialNode					m_Matrial;
	
	
public:
	// cSceneNode

	virtual BOOL		LoadObjInfo(cASEParser* pParser);

			

	// �ش� ���ؽ��� ����Ʈ�� �߰��ϰ� �߰��� �ε����� ���. �����ϴ� ���ؽ� �̸� �����ϴ� �ε��� ���
			void		InsertNormalVertex(NORMALVERTEX* pInput,int& outIndex);
			void		InsertBLENDVERTEX( BLENDVERTEX* pInput,int& outIndex );

	// ���ؽ��� ���� ��ǥ��� ��ȯ�Ѵ�.
			void		TransformVertexOnLocalCoord();		

	// �����ϴ� ���� �������� �߰��Ѵ�.
			void		InsertBoneRefName(LPCSTR strNodeName);
	
	// ��Ʈ���� ���� ���� ã�� ã�� �����Ѵ�.(���� ������ ��� ���� ���������� �����Ŀ� �����Ѵ�.) 
			void		LinkToBone();

			BOOL		GetIsBone() const { return m_bIsBone; }
			void		SetIsBone(BOOL val) { m_bIsBone = val; }		

			BOOL		IsSkinnedMesh();

	cSphere*			GetBoundingSphere();
	void				CreateBoundingSphere();

	const D3DXVECTOR3&	GetVelocity()  { return m_vecVelocity; }
	const D3DXVECTOR3&	GetPosition()  { return m_Position; }

	void				SetMatrial(cMaterialNode val) { m_Matrial = val; }
	

	virtual void		Init();
	virtual void		Uninit();

	virtual void		Update();
	virtual void		Render();


};



