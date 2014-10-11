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


class cMeshNode:
	public cSceneNode
{
public:
	cMeshNode(void);
	virtual ~cMeshNode(void);
protected:	
	cRscIndexBuffer*		m_pRscIndexBuffer;	
	cRscVertexBuffer*		m_pRscVetextBuffer;	
	unsigned short			m_primitiveCount;
	unsigned short			m_startIndex;
	unsigned char			m_materialRefIndex;
	unsigned char			m_materialSubIndex;

public:
	virtual void			Render();
	virtual	void			BuildComposite(Entity* pEntity);


	void					SetRscIndexBuffer(cRscIndexBuffer* val);
	void					SetRscVertextBuffer(cRscVertexBuffer* val);
	virtual void			QueueRenderer(Entity* pEntity,bool bTraversal);

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

	Material* GetMaterial(unsigned char subIndex);
	const std::vector<Material*>& GetMaterials();

	unsigned short GetStartIndex() const { return m_startIndex; }
	void SetStartIndex(unsigned short val) { m_startIndex = val; }
	unsigned char GetMaterialRefIndex() const { return m_materialRefIndex; }
	void SetMaterialRefIndex(unsigned char val) { m_materialRefIndex = val; }
	unsigned short GetPrimitiveCount() const { return m_primitiveCount; }
	void SetPrimitiveCount(unsigned short val) { m_primitiveCount = val; }
	unsigned char GetMaterialSubIndex() const { return m_materialSubIndex; }
	void SetMaterialSubIndex(unsigned char val) { m_materialSubIndex = val; }

	void RenderIsntancing();

	cRscIndexBuffer* GetRscIndexBuffer() const { return m_pRscIndexBuffer; }
	cRscVertexBuffer* GetRscVetextBuffer() const { return m_pRscVetextBuffer; }
};

}