#pragma once

#include "Math/Sphere.h"
#include "Graphics/RscVertexBuffer.h"
#include "Graphics/RscIndexBuffer.h"
#include "SceneNode.h"
#include "Graphics/MaterialEx.h"
#include "Graphics/Vertex.h"

namespace Sophia
{

class MatrixStreamVertexBuffer;
class cMeshNode;
class cView;
class VertexTexture;
class VertexInstancingBuffer;
class MatrixTexture;
class IndexInstancingBuffer;
class InstanceDataBuffer;
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
	Material*				m_pMaterial;
	bool					m_bInstancingEnable;

	MatrixTexture*			m_pMatrixInstancingTexture;		// world Matrix
	InstanceDataBuffer*			m_pInstanceDataBuffer;


public:
	virtual void			Render(DWORD elapseTime);
	virtual	void			BuildComposite(Entity* pEntity);


	void					SetRscIndexBuffer(cRscIndexBuffer* val);
	void					SetRscVertextBuffer(cRscVertexBuffer* val);
	virtual void			GatherRender(Entity* pEntity,bool bTraversal);
	virtual void			Release();

	// ISerialize
	virtual void		SerializeIn(std::ifstream& stream);
	virtual void		SerializeOut(std::ofstream& stream);

	virtual void SerializeOutMesh(std::ofstream& stream);
	virtual void SerializeInMesh(std::ifstream& stream);

	Material* GetMaterial() const { return m_pMaterial; }	
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

	virtual void RenderInstancing(int instanceSize );

	virtual void UpdateMatrixInstancing(std::vector<cMeshNode*>& list, const D3DLOCKED_RECT& lock);

	cRscIndexBuffer* GetRscIndexBuffer() const { return m_pRscIndexBuffer; }
	cRscVertexBuffer* GetRscVetextBuffer() const { return m_pRscVetextBuffer; }
	bool GetInstancingEnable() const { return m_bInstancingEnable; }
	void ChangeInstancingEnable(bool val);
	MatrixTexture*			GetMatrixInstancingTexture() const { return m_pMatrixInstancingTexture; }
	InstanceDataBuffer* GetInstanceDataBuffer() const { return m_pInstanceDataBuffer; }
protected:
	virtual void CreateInstancingResource();
	virtual void ReleaseInstancingResource();
};

}