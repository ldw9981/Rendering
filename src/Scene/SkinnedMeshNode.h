#pragma once
#include "meshnode.h"

struct BONEREFINFO
{
	string strNodeName;
	cMeshNode* pRefBoneMesh;		//���� ������ �޽��̴�	
	D3DXMATRIX	BoneOffSetTM_INV;

	BONEREFINFO()
	{
		pRefBoneMesh=NULL;
		D3DXMatrixIdentity(&BoneOffSetTM_INV);
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
	vector<BONEREFINFO>		m_vecBoneRef;				//�޽��� �����ϴ� �� ����
	D3DXMATRIX*				m_pArrayMatBoneRef;

public:
	virtual void			Render();	
	virtual void			BuildComposite();

	void					SetBoneRef(vector<BONEREFINFO>& vecBoneRef);
	vector<BONEREFINFO>&	GetArrayBoneRef()  { return m_vecBoneRef; }
	void					LinkToBone();


	virtual void			QueueRenderer();
};
