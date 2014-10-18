#pragma once
#include "meshnode.h"
namespace Sophia
{

class MultiSub;
class Material;
class IndexStreamVertexBuffer;
class BoneStreamTexture;

struct BONEREFINFO
{
	std::string strNodeName;
	cSceneNode* pNode;		//본은 무조건 메쉬이다	
	D3DXMATRIX	SkinOffset;

	BONEREFINFO()
	{
		pNode=NULL;
		D3DXMatrixIdentity(&SkinOffset);
	}
};

struct BONEWEIGHT
{
	BYTE	bone_index;
	float	bone_weight;		
	static BOOL GreatBoneWeight(BONEWEIGHT& a,BONEWEIGHT& b)
	{
		if ( a.bone_weight > b.bone_weight)
			return TRUE;

		return FALSE;
	}
};

class SkinnedMeshNode :
	public cMeshNode
{
public:
	SkinnedMeshNode(void);
	virtual ~SkinnedMeshNode(void);

protected:
	std::vector<BONEREFINFO>		m_vecBoneRef;				//메쉬가 참조하는 본 정보
	D3DXMATRIX*					m_pMatrixPallete;
	bool						m_updateBlendMatrix;
	IndexStreamVertexBuffer*	m_pIndexStreamVertexBuffer;
	BoneStreamTexture*			m_pBoneStreamTexture;


public:
	virtual void			Update( DWORD elapseTime );
	virtual void			Render();
	virtual void			BuildComposite(Entity* pEntity);

	void					SetBoneRef(std::vector<BONEREFINFO>& vecBoneRef);
	std::vector<BONEREFINFO>&	GetArrayBoneRef()  { return m_vecBoneRef; }
	void					LinkToBone(Entity* pEntity);


	virtual void			QueueRenderer(Entity* pEntity,bool bTraversal);
	virtual	void			Release();

	// ISerialize
	virtual void		SerializeIn(std::ifstream& stream);
	virtual void		SerializeOut(std::ofstream& stream);

	virtual void SerializeOutMesh(std::ofstream& stream);
	virtual void SerializeInMesh(std::ifstream& stream);

	void UpdateMatrixPallete();
	D3DXMATRIX* GetMatrixPallete() const { return m_pMatrixPallete; }
	BoneStreamTexture* GetBoneStreamTexture() const { return m_pBoneStreamTexture; }
	IndexStreamVertexBuffer* GetIndexStreamVertexBuffer() const { return m_pIndexStreamVertexBuffer; }	
protected:
	virtual void CreateInstancingResource();
	virtual void ReleaseInstancingResource();
};
}