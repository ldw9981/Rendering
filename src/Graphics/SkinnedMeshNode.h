#pragma once
#include "meshnode.h"
namespace Sophia
{

class MultiSub;
class Material;
class cRscTexture;


struct BONEREFINFO
{
	// BoneSceneNode ���� ��ǥ�迡���� SkinnedMesh�� Transform �� Relative Transform(Local)�̴�.
	// SkinnedMesh�� �׸��� ����� ���� WorldTransform = Relative Transform(Local) * �ش� BoneNode�� WorldTransform
	D3DXMATRIX	SkinnedMeshOffset;		
	cSceneNode* pBoneSceneNode;			
	std::string strNodeName;

	BONEREFINFO()
	{
		pBoneSceneNode=NULL;
		D3DXMatrixIdentity(&SkinnedMeshOffset);
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
	std::vector<BONEREFINFO>		m_vecBoneRef;				//�޽��� �����ϴ� �� ����

	bool						m_updateMatrixPallete;
	cRscTexture*				m_pMatrixPalleteTexture;
public:
	virtual void			Update( DWORD elapseTime );
	virtual void			Render(DWORD elapseTime);
	virtual void			BuildComposite(Entity* pEntity);

	void					SetBoneRef(std::vector<BONEREFINFO>& vecBoneRef);
	std::vector<BONEREFINFO>&	GetArrayBoneRef()  { return m_vecBoneRef; }
	void					LinkToBone(Entity* pEntity);


	virtual void			GatherRender(Entity* pEntity,bool bTraversal);
	virtual	void			Release();

	// ISerialize
	virtual void		SerializeIn(std::ifstream& stream);
	virtual void		SerializeOut(std::ofstream& stream);

	virtual void SerializeOutMesh(std::ofstream& stream);
	virtual void SerializeInMesh(std::ifstream& stream);

	void UpdateMatrixPallete();
	
	virtual void RenderInstancing( int instanceSize );
	virtual void UpdateMatrixInstancing( std::vector<cMeshNode*>& list, const D3DLOCKED_RECT& lock);


	void ChangeInstancingEnable(bool val);
protected:
	virtual void CreateInstancingResource();
	void CreateMatrixPallete();
	void DeleteMatrixPallete();
};
}