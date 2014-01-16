#pragma once
#include "Foundation/interface.h"
#include "Math/Sphere.h"
#include "Graphics/RscVertexBuffer.h"
#include "Graphics/RscIndexBuffer.h"
#include "SceneNode.h"
#include "Graphics/MaterialEx.h"
#include "Graphics/Vertex.h"

class cMeshNode;
class cView;

class cMeshNode:
	public cSceneNode
{
public:
	cMeshNode(void);
	virtual ~cMeshNode(void);
protected:	
	D3DXMATRIX				m_worldReference;
	BOOL					m_bIsBone;
	int						m_nStartIndex;
	int						m_nPrimitiveCount;
	Material				m_Matrial;
	cRscIndexBuffer*		m_pRscIndexBuffer;	
	cRscVertexBuffer*		m_pRscVetextBuffer;	
	std::vector<cMeshNode*>		m_vecSubMesh;
public:
	virtual void			Update(DWORD elapseTime);
	virtual void			Render();
	virtual	void			BuildComposite(Entity* pEntity);

	void					SetStartIndex(WORD val) { m_nStartIndex = val; }
	void					SetPrimitiveCount(WORD val) { m_nPrimitiveCount = val; }	
	
	void					SetRscIndexBuffer(cRscIndexBuffer* val);

	void					SetRscVertextBuffer(cRscVertexBuffer* val);

	BOOL					GetIsBone() const { return m_bIsBone; }
	void					SetIsBone(BOOL val) { m_bIsBone = val; }		

	void					SetMatrial(Material& val) { m_Matrial = val; }	


	void					AddMultiSub(cMeshNode* mesh);	



	virtual void			QueueRenderer(Entity* pEntity,bool bTraversal);
	virtual void			QueueRendererShadow(Entity* pEntity,bool bTraversal);

	void					CalculateTangentBinormal();
	void                    CalculateVector(const D3DXVECTOR3& vertex1,const D3DXVECTOR3& vertex2,const D3DXVECTOR3& vertex3,
		const TEXCOORD& t1,const TEXCOORD& t2,const TEXCOORD& t3,
		D3DXVECTOR3& tangent1,D3DXVECTOR3& tangent2,D3DXVECTOR3& tangent3,
		D3DXVECTOR3& binormal1,D3DXVECTOR3& binormal2,D3DXVECTOR3& binormal3);

	virtual void			Release();

	D3DXMATRIX&				GetWorldReference();
};



