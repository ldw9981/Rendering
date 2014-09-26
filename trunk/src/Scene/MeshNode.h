#pragma once

#include "Math/Sphere.h"
#include "Graphics/RscVertexBuffer.h"
#include "Graphics/RscIndexBuffer.h"
#include "SceneNode.h"
#include "Graphics/MaterialEx.h"
#include "Graphics/Vertex.h"

namespace Sophia
{


class cMeshNode;
class cView;

class MultiSub
{
public:
	unsigned short primitiveCount;
	unsigned short startIndex;
	unsigned char materialIndex;
};

class cMeshNode:
	public cSceneNode
{
public:
	cMeshNode(void);
	virtual ~cMeshNode(void);
protected:	
	cRscIndexBuffer*		m_pRscIndexBuffer;	
	cRscVertexBuffer*		m_pRscVetextBuffer;	
	std::vector<MultiSub>	m_vecMultiSub;
	std::vector<MeshMaterials*>		m_vecSceneMaterial;
	MeshMaterials*			m_pMeshMaterials;
public:
	virtual void			Render(MultiSub* pMultiSub,Material* pMaterial);
	virtual	void			BuildComposite(Entity* pEntity);


	void					SetRscIndexBuffer(cRscIndexBuffer* val);
	void					SetRscVertextBuffer(cRscVertexBuffer* val);

	void					AddMultiSub(WORD startIndex,WORD primitiveCount,unsigned char materialIndex);	


	virtual void			QueueRenderer(Entity* pEntity,bool bTraversal);
	virtual void			QueueRendererShadow(Entity* pEntity,bool bTraversal);

	void					CalculateTangentBinormal();
	void                    CalculateVector(const D3DXVECTOR3& vertex1,const D3DXVECTOR3& vertex2,const D3DXVECTOR3& vertex3,
		const TEXCOORD& t1,const TEXCOORD& t2,const TEXCOORD& t3,
		D3DXVECTOR3& tangent1,D3DXVECTOR3& tangent2,D3DXVECTOR3& tangent3,
		D3DXVECTOR3& binormal1,D3DXVECTOR3& binormal2,D3DXVECTOR3& binormal3);

	virtual void			Release();

	// ISerialize
	virtual void		SerializeIn(std::ifstream& stream);
	virtual void		SerializeOut(std::ofstream& stream);

	virtual void SerializeOutMesh(std::ofstream& stream);
	virtual void SerializeInMesh(std::ifstream& stream);

	virtual void PushMaterial(EntityMaterials* pEntityMaterial);
	virtual void PopMaterial();
	MeshMaterials* GetMeshMaterials();

	Material* GetMaterial(MultiSub* pMultiSub);

	//virtual void GetRenderPosition(D3DXVECTOR3& pos);
	virtual const D3DXVECTOR3* GetRenderWorldPos();;
};

}