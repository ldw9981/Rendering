#pragma once
#include "Foundation/interface.h"
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


class cMeshNode:
	public cSceneNode
{
public:
	cMeshNode(void);
	virtual ~cMeshNode(void);

	typedef struct {
		unsigned short primitiveCount;
		unsigned short startIndex;
		unsigned char materialIndex;
	} MultiSub;
protected:	
	cRscIndexBuffer*		m_pRscIndexBuffer;	
	cRscVertexBuffer*		m_pRscVetextBuffer;	
	std::vector<MultiSub>	m_vecMultiSub;
	std::vector<SceneMaterial*>		m_vecSceneMaterial;
public:
	virtual void			Update(DWORD elapseTime);
	virtual void			Render(unsigned char multiSubIndex);
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

	virtual void PushMaterial(EntityMaterial* pEntityMaterial);
	virtual void PopMaterial();
};

}